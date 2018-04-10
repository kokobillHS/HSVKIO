/*
 * apartment.cpp
 *
 * Created: 04.10.2016 12:43:06
 *  Author: dmitry
 */ 

#include <avr/io.h>
#include "include/apartment.h"
#include "include/devices.h"
#include "include/digitalio.h"
#include "include/time.h"
#include "include/pindefs.h"
#include "include/persistence.h"

#define CONFIGURABLE_DEV_COUNT 24

static uint8_t occuStates[CONFIGURABLE_DEV_COUNT] =  { 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 };
static uint8_t awayStates[CONFIGURABLE_DEV_COUNT] =  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
static uint8_t delayedAwayStates[CONFIGURABLE_DEV_COUNT] =  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 };
static uint8_t unoccuStates[CONFIGURABLE_DEV_COUNT] = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
static uint8_t memorizedStates[CONFIGURABLE_DEV_COUNT] = { 0 };
static uint8_t currentMode = MODE_OCCUPIED;
static uint8_t onStatesCount[CONFIGURABLE_DEV_COUNT] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
//static uint8_t radiatorState;
//static uint8_t deviceState[4] = { 0 };
static uint32_t offDelayValue = 10;
static uint32_t delayStartTime = 0;
static bool motionEnabled = true;
static bool motionDisabledByMode = false;
static uint32_t modeDelayStartTime = 0;

static bool occupancyButtonsEnabledFlag = true;
/*
#define ELEVATOR_ARRIVE_TIME 3000L

static long elevatorCallTime;
static bool elevatorWasCalled = false;
static bool elevatorHasArrived = false;
*/
struct specialModeType {
    bool winter;
    bool intrusion;
    bool fire;
    bool flood;
    bool elevator;
};

specialModeType specialModes;

#define MEMORIZE_STATE(i) (MEMORIZE + onStatesCount[i])
#define RECALL_STATE(i) (RECALL + onStatesCount[i])
#define DELAYEDOFF_STATE(i) (DELAYED_OFF + onStatesCount[i])

void setOccupiedMode() {
    for (uint8_t i = 0; i < CONFIGURABLE_DEV_COUNT; i++) {
        // "off" + all available "on" states are processed as is
        if (occuStates[i] <= onStatesCount[i]) { 
            setDeviceState(i, occuStates[i]);
        } else if (occuStates[i] == RECALL_STATE(i)) {
            setDeviceState(i, memorizedStates[i]);
        }
    }
    
    currentMode = MODE_OCCUPIED;
}

void setAwayMode() {
    for (uint8_t i = 0; i < CONFIGURABLE_DEV_COUNT; i++) {
        // "off" + all available "on" states are processed as is
        if (awayStates[i] <= onStatesCount[i]) {
            setDeviceState(i, awayStates[i]);
        } else if (awayStates[i] == MEMORIZE_STATE(i)) {
            memorizedStates[i] = getDeviceState(i);
            setDeviceState(i, 0);
        }
    }

    currentMode = MODE_AWAY;
}

void setUnoccupiedMode() {
    for (uint8_t i = 0; i < CONFIGURABLE_DEV_COUNT; i++) {
        // "off" + all available "on" states are processed as is
        if (unoccuStates[i] <= onStatesCount[i]) {
            setDeviceState(i, unoccuStates[i]);
            memorizedStates[i] = 0;
        } else if (unoccuStates[i] == MEMORIZE_STATE(i)) {
            memorizedStates[i] = getDeviceState(i);
            setDeviceState(i, 0);
        }
    }

    currentMode = MODE_UNOCCUPIED;
}

void setDelayedAwayMode() {
    for (uint8_t i = 0; i < CONFIGURABLE_DEV_COUNT; i++) {
        // "off" + all available "on" states are processed as is
        if (awayStates[i] <= onStatesCount[i]) {
            setDeviceState(i, awayStates[i]);
        } else if (awayStates[i] == MEMORIZE_STATE(i)) {
            memorizedStates[i] = getDeviceState(i);
            setDeviceState(i, 0);
        }
    }

    currentMode = MODE_AWAY;
    delayStartTime = millis();
}

void setCurrentMode(uint8_t mode) {
    if (mode == MODE_OCCUPIED) {
        setOccupiedMode();
    } else if (mode == MODE_AWAY) {
        setAwayMode();
    } else if (mode == MODE_UNOCCUPIED) {
        setUnoccupiedMode();
    } else if (mode == MODE_DELAYED_AWAY) {
        setAwayMode();//setDelayedAwayMode();
    }

    writeCurrentMode(mode);
}

uint8_t getCurrentMode() {
    return currentMode;
}
/*
void callElevator() {
    elevatorCallTime = millis();
    elevatorWasCalled = true;
    elevatorHasArrived = false;
}

bool elevatorCalled() {
    if (elevatorHasArrived) {
        return false;
    } else if (elevatorWasCalled) {
        if (millis() - elevatorCallTime > ELEVATOR_ARRIVE_TIME) {
            elevatorHasArrived = true;
            elevatorWasCalled = false;
            return false;
        }
    }

    return elevatorWasCalled;
}

bool elevatorArrived() {
    if (elevatorHasArrived) {
        return true;
    } else if (elevatorWasCalled) {
        if (millis() - elevatorCallTime > ELEVATOR_ARRIVE_TIME) {
            elevatorHasArrived = true;
            elevatorWasCalled = false;
            return true;
        }
    }

    return false;
}
*/
void setSpecialMode(uint8_t mode, bool state) {
    switch (mode) {
        case FLOOD:
            specialModes.flood = state;
            break;
        case FIRE:
            specialModes.fire = state;
            break;
        case INTRUSION:
            specialModes.intrusion = state;
            break;
        case WINTER:
            specialModes.winter = state;
            break;
        case ELEVATOR:
            specialModes.elevator = state;
            break;
    }
}

bool specialModeActive(uint8_t mode) {
    switch (mode) {
        case FLOOD:
            return specialModes.flood;
        case FIRE:
            return specialModes.fire;
        case INTRUSION:
            return specialModes.intrusion;
        case WINTER:
            return specialModes.winter;
        case ELEVATOR:
            return specialModes.elevator;
    }

    return false;
}

void setOccuDeviceState(uint8_t device, uint8_t state) {
    if (device >= CONFIGURABLE_DEV_COUNT) {
        return;
    }

    occuStates[device] = state;
    updateSingleSetting(device, state, MODE_OCCUPIED);

    if (state == MEMORIZE_STATE(device)) {
        awayStates[device] = RECALL_STATE(device);
        updateSingleSetting(device, RECALL_STATE(device), MODE_AWAY);
    }
}

void setAwayDeviceState(uint8_t device, uint8_t state) {
    if (device >= CONFIGURABLE_DEV_COUNT) {
        return;
    }

    awayStates[device] = state;
    updateSingleSetting(device, state, MODE_AWAY);

    if (state == RECALL_STATE(device)) {
        occuStates[device] = MEMORIZE_STATE(device);
        updateSingleSetting(device, MEMORIZE_STATE(device), MODE_OCCUPIED);
    }
}

void setUnoccuDeviceState(uint8_t device, uint8_t state) {
    if (device >= CONFIGURABLE_DEV_COUNT) {
        return;
    }

    unoccuStates[device] = state;
    updateSingleSetting(device, state, MODE_UNOCCUPIED);
}

uint8_t getOccuDeviceState(uint8_t device) {
    if (device >= CONFIGURABLE_DEV_COUNT) {
        return 0xFF; // no such device
    }

    return occuStates[device];
}

uint8_t getAwayDeviceState(uint8_t device) {
    if (device >= CONFIGURABLE_DEV_COUNT) {
        return 0xFF; // no such device
    }   

    return awayStates[device];
}

uint8_t getUnoccuDeviceState(uint8_t device) {
    if (device >= CONFIGURABLE_DEV_COUNT) {
        return 0xFF; // no such device
    }

    return unoccuStates[device];
}

void memorizeDeviceState(uint8_t device) {
    memorizedStates[device] = getDeviceState(device);
}

void recallDeviceState(uint8_t device) {
    setDeviceState(device, memorizedStates[device]);
}

void setDeviceState(uint8_t device, uint8_t state) {
    switch (device) {
        case LightsHall:
            output(OUT_LightsHall, state);
            break;
        case LightsBathroom:
            output(OUT_LightsBathroom, state);
            break;
        case LightsMirrorBath:
            output(OUT_LightsMirrorBath, state);
            break;
        case FanBathroom:
            output(OUT_FanBathroom, state);
            break;
        case LightsKitchen:
            output(OUT_LightsKitchen, state);
            break;
        case LightsCookZone:
            output(OUT_LightsCookZone, state);
            break;
        case LightsBalcony:
            output(OUT_LightsBalcony, state);
            break;
        case LightsRoom1_1:
            output(OUT_LightsRoom1_1, state);
            break;
        case LightsRoom1_2:
            output(OUT_LightsRoom1_2, state);
            break;
        case LightsRoom2_1:
            output(OUT_LightsRoom2_1, state);
            break;
        case LightsRoom2_2:
            output(OUT_LightsRoom2_2, state);
            break;
        case Oven:
            output(OUT_Oven, state);
            break;
        case WashingMachine:
            output(OUT_WashingMachine, state);
            break;
        case Dishwasher:
            output(OUT_Dishwasher, state);
            break;
        case Boiler:
            output(OUT_Boiler, state);
            break;
        case FloorHeaterToilet:
            output(OUT_FloorHeaterToilet, state);
            break;
        case Radiators:
            output(OUT_Radiators, state);
            break;
        case SocketsRoom1:
            output(OUT_SocketsRoom1, state);
            break;
        case SocketsRoom2:
            output(OUT_SocketsRoom2, state);
            break;
        case SocketsKitchen:
            output(OUT_SocketsKitchen, state);
            break;
        case SocketsBathroom:
            output(OUT_SocketsBathroom, state);
            break;
        case ForceValves:
            output(OUT_ForceValves, state);
            break;
        case FloorHeaterHall:
            output(OUT_FloorHeaterHall, state);
            break;
    }
}

uint8_t getDeviceState(uint8_t device) {
    switch (device) {
        case LightsHall:
            return outputState(OUT_LightsHall);
        case LightsBathroom:
            return outputState(OUT_LightsBathroom);
        case LightsMirrorBath:
            return outputState(OUT_LightsMirrorBath);
        case FanBathroom:
            return outputState(OUT_FanBathroom);
        case LightsKitchen:
            return outputState(OUT_LightsKitchen);
        case LightsCookZone:
            return outputState(OUT_LightsCookZone);
        case LightsBalcony:
            return outputState(OUT_LightsBalcony);
        case LightsRoom1_1:
            return outputState(OUT_LightsRoom1_1);
        case LightsRoom1_2:
            return outputState(OUT_LightsRoom1_2);
        case LightsRoom2_1:
            return outputState(OUT_LightsRoom2_1);
        case LightsRoom2_2:
            return outputState(OUT_LightsRoom2_2);
        case WashingMachine:
            return outputState(OUT_WashingMachine);
        case Boiler:
            return outputState(OUT_Boiler);
        case Oven:
            return outputState(OUT_Oven);
        case FloorHeaterHall:
            return outputState(OUT_FloorHeaterHall);
        case FloorHeaterToilet:
            return outputState(OUT_FloorHeaterToilet);
        case Radiators:
            return outputState(OUT_Radiators);
        case Dishwasher:
            return outputState(OUT_Dishwasher);
        case SocketsBathroom:
            return outputState(OUT_SocketsBathroom);
        case SocketsKitchen:
            return outputState(OUT_SocketsKitchen);
        case SocketsRoom1:
            return outputState(OUT_SocketsRoom1);
        case SocketsRoom2:
            return outputState(OUT_SocketsRoom2);
        case ForceValves:
            return outputState(OUT_ForceValves);
    }

    return 0;
}

void turnEverythingOff() {
    for (uint8_t i = FIRST_DEVICE_ID; i <= LAST_DEVICE_ID; i++) {
        setDeviceState(i, 0);
    }
}


void setOccupancyButtonsEnableFlag(bool flag) {
    occupancyButtonsEnabledFlag = flag;
}

bool occupancyButtonsEnabled() {
    return occupancyButtonsEnabledFlag;
}

static bool lobbyToggleScheduled = false;
static uint32_t lastLobbyToggle = millis();

void processLights() {    
    static bool toggledByMotion = false;
    static bool motionDisabledByButton = false;
    static uint32_t lastMotionDetected = millis();
    static uint32_t motionDelay = 60000L;

            
    if (lobbyToggleScheduled && (millis() - lastLobbyToggle > 1000)) {
        toggle(OUT_LightsHall);
        lobbyToggleScheduled = false;
    }

    if (motionDisabledByMode && (millis() - modeDelayStartTime > 10000)) {
        setMotionSensorPermission(true);
        motionDisabledByMode = false;
    }

    if (!lobbyToggleScheduled) {
        if (!held(IN_MotionSensor) && motionEnabled) {
            turnOn(OUT_LightsHall);
            lastMotionDetected = millis();
            toggledByMotion = true;
        }

        if (toggledByMotion && motionEnabled && (millis() - lastMotionDetected > motionDelay)) {
            turnOff(OUT_LightsHall);
        }

        if (pressed(IN_ButtonLightsHall)) {
            if (outputState(OUT_LightsHall)) {
                if (motionEnabled) {
                    motionEnabled = false;
                    motionDisabledByButton = true;
                }

                turnOff(OUT_LightsHall);            
                toggledByMotion = false;
            } else {
                turnOn(OUT_LightsHall);
                toggledByMotion = false;

                if (motionDisabledByButton) {
                    motionEnabled = true;
                    motionDisabledByButton = false;
                }
            }
            // toggle(OUT_LightsHall);        
        }
    }

    if (pressed(IN_ButtonLightsKitchen)) {
        toggle(OUT_LightsKitchen);
    }

    if (pressed(IN_ButtonLightsCookZone)) {
        toggle(OUT_LightsCookZone);
    }
   
    if (pressed(IN_ButtonLightsBalcony)) {
        toggle(OUT_LightsBalcony);
    }

    if (pressed(IN_ButtonLightsBathroom)) {
        toggle(OUT_LightsBathroom);
    }

    if (pressed(IN_ButtonLightsMirrorBath)) {
        toggle(OUT_LightsMirrorBath);
    }

    if (pressed(IN_ButtonFanBathroom)) {
        toggle(OUT_FanBathroom);
    }

    if (pressed(IN_ButtonLightsRoom1_1)) {
        toggle(OUT_LightsRoom1_1);
    }

    if (pressed(IN_ButtonLightsRoom1_2)) {
        toggle(OUT_LightsRoom1_2);
    }
   
    if (pressed(IN_ButtonLightsRoom2_1)) {
        toggle(OUT_LightsRoom2_1);
    }
   
    if (pressed(IN_ButtonLightsRoom2_2)) {
        toggle(OUT_LightsRoom2_2);
    }
}

/*
void processHouseheldAppliances() {
    if (held(IN_SwitchIron)) {
        output(OUT_Iron, getDeviceState(Iron));
    } else {
        output(OUT_Iron, 0);
    }

    // the oven switch is connected directly to LED
    output(OUT_Oven, getDeviceState(Oven));
    
    if (held(IN_SwitchTV)) {
        output(OUT_TV, getDeviceState(TV));
    } else {
        output(OUT_TV, 0);
    }

    if (held(IN_SwitchAC)) {
        output(OUT_AC, getDeviceState(AC));
    } else {
        output(OUT_AC, 0);
    }
}
*/

void processModes() {
    if (pressed(IN_ButtonOccu)) {
        if (occupancyButtonsEnabled() && (getCurrentMode() != MODE_OCCUPIED)) {
            setCurrentMode(MODE_OCCUPIED);
            //toggle(OUT_LightsHall);
            //lobbyToggleScheduled = true;
            //lastLobbyToggle = millis();
        }
    }

    if (pressed(IN_ButtonAway)) {
        if (occupancyButtonsEnabled()) {
            setCurrentMode(MODE_AWAY);
            modeDelayStartTime = millis();
            motionDisabledByMode = true;
            setMotionSensorPermission(false);
            //toggle(OUT_LightsHall);
            //lobbyToggleScheduled = true;
            //lastLobbyToggle = millis();
        }
    }

    if (pressed(IN_ButtonUnoccu)) {
        if (occupancyButtonsEnabled()) {
            setCurrentMode(MODE_UNOCCUPIED);
            modeDelayStartTime = millis();
            motionDisabledByMode = true;
            setMotionSensorPermission(false);
            //toggle(OUT_LightsHall);
            //lobbyToggleScheduled = true;
            //lastLobbyToggle = millis();
        }
    }

    if (pressed(IN_ButtonAwayDelayed) && (getCurrentMode() == MODE_OCCUPIED)
            && occupancyButtonsEnabled()) {
        //setCurrentMode(MODE_DELAYED_AWAY);
        modeDelayStartTime = millis();
        motionDisabledByMode = true;
        setMotionSensorPermission(false);
        toggle(OUT_LightsHall);
        lobbyToggleScheduled = true;
        lastLobbyToggle = millis();
    }
    /*
    if (!held(IN_FireSensor)) {
        setSpecialMode(FIRE, true);
    } else {
        setSpecialMode(FIRE, false);
    }
    */
}

/*
void outputLedStates() {
    static uint32_t lastToggle = millis();
    static bool toggleThisTime = false;

    if (millis() - lastToggle > 500) {
        toggleThisTime = true;
        lastToggle = millis();
    } else {
        toggleThisTime = false;
    }

    if (ledStates[INDEX_LED_GREEN] == 1) {
        turnOn(OUT_LedGreen);
    } else if (ledStates[INDEX_LED_GREEN] == 0) {
        turnOff(OUT_LedGreen);
    } else if ((ledStates[INDEX_LED_GREEN] == 2) && toggleThisTime) {
        toggle(OUT_LedGreen);
    }

    if (ledStates[INDEX_LED_YELLOW] == 1) {
        turnOn(OUT_LedYellow);
    } else if (ledStates[INDEX_LED_YELLOW] == 0) {
        turnOff(OUT_LedYellow);
    } else if ((ledStates[INDEX_LED_YELLOW] == 2) && toggleThisTime) {
        toggle(OUT_LedYellow);
    }

    if (ledStates[INDEX_LED_RED] == 1) {
        turnOn(OUT_LedRed);
    } else if (ledStates[INDEX_LED_RED] == 0) {
        turnOff(OUT_LedRed);
    } else if ((ledStates[INDEX_LED_RED] == 2) && toggleThisTime) {
        toggle(OUT_LedRed);
    }

    if (ledStates[INDEX_LED_BLUE] == 1) {
        turnOn(OUT_LedBlue);
    } else if (ledStates[INDEX_LED_BLUE] == 0) {
        turnOff(OUT_LedBlue);
    } else if ((ledStates[INDEX_LED_BLUE] == 2) && toggleThisTime) {
        toggle(OUT_LedBlue);
    }
}
*/

void processDelayedStates() {
    if ((getCurrentMode() == MODE_DELAYED_AWAY) && (millis() - delayStartTime > offDelayValue)) {
        for (uint8_t i = 0; i < CONFIGURABLE_DEV_COUNT; i++) {
            if (delayedAwayStates[i] == DELAYEDOFF_STATE(i)) {
                setDeviceState(i, 0);
            }
        }
    }
}

void processIO() {
    // static long delayStartTime = 0;

    if (!specialModeActive(FIRE)) {
        processModes();
        processLights();
        processDelayedStates();
    } else {
        turnEverythingOff();
    }
}

void setMotionSensorPermission(bool permission) {
    motionEnabled = permission;
}

bool getMotionSensorPermission() {
    return motionEnabled;
}

void readSettingsFromStorage() {
    readSettings(occuStates, CONFIGURABLE_DEV_COUNT, MODE_OCCUPIED);
    readSettings(unoccuStates, CONFIGURABLE_DEV_COUNT, MODE_UNOCCUPIED);
    readSettings(awayStates, CONFIGURABLE_DEV_COUNT, MODE_AWAY);
    readSettings(delayedAwayStates, CONFIGURABLE_DEV_COUNT, MODE_DELAYED_AWAY);
}

void writeSettingsToStorage() {
    writeSettings(occuStates, CONFIGURABLE_DEV_COUNT, MODE_OCCUPIED);
    writeSettings(unoccuStates, CONFIGURABLE_DEV_COUNT, MODE_UNOCCUPIED);
    writeSettings(awayStates, CONFIGURABLE_DEV_COUNT, MODE_AWAY);
    writeSettings(delayedAwayStates, CONFIGURABLE_DEV_COUNT, MODE_DELAYED_AWAY);
}
/*
 * ApartmentTest.cpp
 *
 * Not working outputs 2, 17 (device #0); 9, 17 (device #1).
 *
 * Created: 28.07.2016 16:36:43
 * Author : Dmytro Briukhatskyi
 */ 

#include "include/main.h"
#include "MB/include/mb.h"
#include "MB/include/mbport.h"
#include "include/devices.h"
#include "include/apartment.h"
#include "include/persistence.h"

bool awayDelayedRequest = false;

int main() {
    initPorts();
    initTimer();
    initEEPROM();
    eMBInit(MB_RTU, 0x01, 0, 9600, MB_PAR_NONE);
    eMBEnable();

    setCurrentMode(readCurrentMode());
    
    uint32_t lastToggle = millis();
    setSpecialMode(WINTER, true);

    while (1) {
        processIO();
        processSpecialModes();

        if ((millis() - lastToggle) > 500) {
            LED_1_TOGGLE;
            lastToggle = millis();
        }
                
        eMBPoll();
    }
}

void processSpecialModes() {    
    /*
    static bool flipFlop = false;
    static long lastToggle = millis();
    */
     
    if (specialModeActive(WINTER) && !specialModeActive(FIRE)) {
        setDeviceState(Radiators, 1);
    } else {
        setDeviceState(Radiators, 0);
    }
    
    /*
    if (specialModeActive(INTRUSION) && !specialModeActive(FIRE) && (millis() - lastToggle > 500)) {
        for (int i = LightsHall; i <= LightsSmallRoom; i++) {
            if (i == LightsBigRoom) {
                setDeviceState(i, flipFlop * 2);
            } else if (i != FanBathroom) {
                setDeviceState(i, flipFlop);
            }
        }

        flipFlop = !flipFlop;
        lastToggle = millis();
    }
    */
    /*
    if (elevatorCalled()) {
        setDeviceState(LedBlue, 1);
        //turnOn(OUT_LedBlue);
    } else {
        setDeviceState(LedBlue, 0);        
        //turnOff(OUT_LedBlue);
    }
    */
}
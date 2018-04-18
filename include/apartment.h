/*
 * apartment.h
 *
 * Created: 04.10.2016 9:12:47
 *  Author: dmitry
 */ 


#ifndef APARTMENT_H_
#define APARTMENT_H_

#define MODE_UNOCCUPIED 0
#define MODE_AWAY 1
#define MODE_OCCUPIED 2
#define MODE_DELAYED_AWAY 3
#define MODE_COUNT 4

#define DELAYED_OFF 3
#define MEMORIZE 2
#define RECALL 2
#define LEAVE_AS_IS 1

#define FLOOD 0
#define FIRE 1
#define INTRUSION 2
#define WINTER 3
#define ELEVATOR 4

void setCurrentMode(uint8_t mode);

//void callElevator();

//bool elevatorCalled();

//bool elevatorArrived();

uint8_t getCurrentMode();

void setSpecialMode(uint8_t mode, bool state);

bool specialModeActive(uint8_t mode);

void setOccuDeviceState(uint8_t device, uint8_t state);

void setAwayDeviceState(uint8_t device, uint8_t state);

void setUnoccuDeviceState(uint8_t device, uint8_t state);

uint8_t getOccuDeviceState(uint8_t device);

uint8_t getAwayDeviceState(uint8_t device);

uint8_t getUnoccuDeviceState(uint8_t device);

void setOccupancyButtonsEnableFlag(bool flag);

bool occupancyButtonsEnabled();

void setDeviceState(uint8_t device, uint8_t state);

void turnEverythingOff();

uint8_t getDeviceState(uint8_t device);

void memorizeDeviceState(uint8_t device);

void recallDeviceState(uint8_t device);

void processIO();

void setMotionSensorPermission(bool permission);

bool getMotionSensorPermission();

void readSettingsFromStorage();

void writeSettingsToStorage();


#endif /* APARTMENT_H_ */
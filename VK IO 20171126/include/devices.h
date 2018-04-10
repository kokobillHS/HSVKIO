/*
 * devices.h
 *
 * Created: 04.10.2016 9:21:47
 *  Author: dmitry
 */ 


#ifndef DEVICES_H_
#define DEVICES_H_

enum DeviceType {
    Oven,           // 1
    Radiators,      // 2
    SocketsKitchen, // 3
    SocketsRoom1,   // 4
    SocketsRoom2,   // 5
    ForceValves,    // 6
    LightsHall,     // 7
    LightsKitchen,  // 8
    LightsCookZone, // 9
    LightsBalcony,  // 10
    LightsBathroom, // 11
    LightsMirrorBath,   //12
    FanBathroom,    // 13
    LightsRoom1_1,  // 14
    LightsRoom1_2,  // 15
    LightsRoom2_1,  // 16
    LightsRoom2_2,  // 17
    Boiler,         // 18
    WashingMachine, // 19
    Dishwasher,     // 20
    SocketsBathroom,    // 21
    FloorHeaterToilet,  // 22
    FloorHeaterHall,    // 23
    DEVICE_TYPE_COUNT
};
/*
    Переделать пожарную логику: сделать массив флагов по количеству устройств.
*/
// to turn off during fire
#define FIRST_DEVICE_ID 0
#define LAST_DEVICE_ID 23

#define OUT_Oven 1
#define OUT_Radiators 2
#define OUT_SocketsKitchen 3
#define OUT_SocketsRoom1 4
#define OUT_SocketsRoom2 5
#define OUT_ForceValves 6
#define OUT_LightsHall 8
#define OUT_LightsKitchen 9
#define OUT_LightsCookZone 10
#define OUT_LightsBalcony 11
#define OUT_LightsBathroom 12
#define OUT_LightsMirrorBath 13
#define OUT_FanBathroom 14
#define OUT_LightsRoom1_1 15
#define OUT_LightsRoom1_2 16
#define OUT_LightsRoom2_1 17
#define OUT_LightsRoom2_2 18
#define OUT_Boiler 19
#define OUT_WashingMachine 20
#define OUT_Dishwasher 21
#define OUT_SocketsBathroom 22
#define OUT_FloorHeaterToilet 23
#define OUT_FloorHeaterHall 24

#define IN_ButtonLightsHall 1
#define IN_ButtonLightsKitchen 2
#define IN_ButtonLightsCookZone 3
#define IN_ButtonLightsBalcony 4
#define IN_ButtonLightsBathroom 5
#define IN_ButtonLightsMirrorBath 6
#define IN_ButtonFanBathroom 7
#define IN_ButtonLightsRoom1_1 8
#define IN_ButtonLightsRoom1_2 9
#define IN_ButtonLightsRoom2_1 10
#define IN_ButtonLightsRoom2_2 11
#define IN_MotionSensor 12
#define IN_ButtonOccu 13
#define IN_ButtonAway 14
#define IN_ButtonUnoccu 15
#define IN_ButtonAwayDelayed 16
#define IN_FireSensor 18

#endif /* DEVICES_H_ */
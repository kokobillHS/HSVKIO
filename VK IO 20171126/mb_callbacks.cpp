/*
 * CPPFile1.cpp
 *
 * Created: 27.07.2016 13:43:00
 *  Author: dmitry
 */ 

 #define F_OSC 8000000
 #define ENUM_START_OFFSET 1

 #include "MB/include/mb.h"
 #include "MB/include/mbutils.h"
 #include "include/digitalio.h"
 #include "include/devices.h"
 #include "include/apartment.h"
 #include "include/persistence.h"

 inline uint8_t bufferIndex(uint8_t bitIndex) {
    return bitIndex / 8;
 }

inline uint8_t bufferOffset(uint8_t bitIndex) {
    return bitIndex & 0x07; // fast version of bitIndex % 8
}

USHORT getHoldingState(uint16_t reg) {
    switch (reg) {
        case 1:
            /* addresses from 1 to 99 are reserved for devices with many possible states */
            break;
        case 100:
            return getCurrentMode();
        case 255:
            return getStartingAddress();
        default:
            if ((reg >= 101) && (reg <= 150)) {
                return getOccuDeviceState(reg - 101);
            } else if ((reg >= 151) && (reg <= 200)) {
                return getUnoccuDeviceState(reg - 151);
            } else if ((reg >= 201) && (reg <= 250)) {
                return getAwayDeviceState(reg - 201);
            }
    }

    return 0;
};

void setHoldingState(USHORT reg, uint8_t state) {
    switch (reg) {
        case 1:
            /* addresses from 1 to 99 are reserved for devices with many possible states */
            break;
        case 100:
            setCurrentMode(state);
            break;
        case 255:
            setStartingAddress(state);
            break;
        default:
            if ((reg >= 101) && (reg <= 150)) {
                setOccuDeviceState(reg - 101, state);
            } else if ((reg >= 151) && (reg <= 200)) {
                setUnoccuDeviceState(reg - 151, state);
            } else if ((reg >= 201) && (reg <= 250)) {
                setAwayDeviceState(reg - 201, state);
            }
    }
};

eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs) {
    eMBErrorCode    eStatus = MB_ENOERR;
    
    return eStatus;
}

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode) {
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT addrBase0 = usAddress - 1;

    if (eMode == MB_REG_READ) {
        for (USHORT i = 0; i < usNRegs; i++) {
            pucRegBuffer[2 * i] = 0;
            pucRegBuffer[2 * i + 1] = getHoldingState(addrBase0 + i);
        }
    } else {
        for (USHORT i = 0; i < usNRegs; i++) {
            setHoldingState(addrBase0 + i, pucRegBuffer[i * 2 + 1]);
        }
    }

    return eStatus;
}

uint8_t coilState(uint8_t i) {
    if (i <= DEVICE_TYPE_COUNT) {
        return !!getDeviceState(i - ENUM_START_OFFSET); // enum elements start from 0
    }

    switch(i) {
        case 101:
            return occupancyButtonsEnabled();
        case 102:
            return specialModeActive(WINTER);
        case 103:
            return getMotionSensorPermission();
   }

    return 0;
}

void setCoilState(uint8_t i, uint8_t state) {
    if (i <= DEVICE_TYPE_COUNT) {
        setDeviceState(i - ENUM_START_OFFSET, state);
    }

    switch(i) {
        case 101:
            setOccupancyButtonsEnableFlag(!!state);
        case 102:
            setSpecialMode(WINTER, true);
        case 103:
            setMotionSensorPermission(state != 0);
    }
}

#include "include/pindefs.h"
eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode) {
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT addrBase0 = usAddress - 1;
    LED_5_TOGGLE;

    if (eMode == MB_REG_READ) {
       for (USHORT i = 0; i < (usNCoils / 8) + !!(usNCoils & 0x0007); i++) {
           pucRegBuffer[i] = 0;
       }
    
       for (USHORT i = 0; i < usNCoils; i++) {
           pucRegBuffer[bufferIndex(i)] |= (coilState(addrBase0 + i) << bufferOffset(i));
       }
    } else if (eMode == MB_REG_WRITE) {
        for (USHORT i = 0; i < usNCoils; i++) {
            setCoilState(addrBase0 + i, !!(pucRegBuffer[bufferIndex(i)] & (1 << bufferOffset(i))));
        }
    }

    return eStatus;
}

uint8_t discreteState(uint8_t i) {
    if ((i > 4) && (i < 13)) {
        return !!outputState(i + 20);
    }
    
    switch (i) {
        case 0:
            return specialModeActive(FLOOD);
        case 1:
            return specialModeActive(FIRE);
        case 2:
            return specialModeActive(INTRUSION);
        case 3: 
            return specialModeActive(WINTER);
    }
    
    return  0;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete) {
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT addrBase0 = usAddress - 1;

    for (USHORT i = 0; i < usNDiscrete; i++) {
        xMBUtilSetBits(pucRegBuffer, i, 1, discreteState(i + addrBase0) );
    }

    return eStatus;
}
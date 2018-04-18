/*
 * digitalio.cpp
 *
 * Created: 18.08.2016 16:27:12
 *  Author: dmitry
 */ 

#include <avr/io.h>
#include "include/digitalio.h"
#include "include/time.h"
#include "include/helpers.h"

//TODO: Replace the following two declarations with read from EEPROM

//TODO: Add constants or macros for defining in/out port and pin
#define OUT_01_PORT PORTC
#define OUT_01_PIN PINC
#define OUT_01_BIT 0


uint32_t shortDelay[INPUTS_COUNT] = { 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2
};

uint32_t longDelay[INPUTS_COUNT] = { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
    1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000
};

volatile bool extModuleStates[8] = { 0 };


int input(uint8_t ordinal) {
	
	if ( ordinal <= 8 ) {
		return (~PIND & (1 << (8-ordinal)));
	}
	
	if ( ordinal <= 16 ) {
		return (~PINE & (1 << (16-ordinal)));
	}
	
	if ( ordinal <= 24 ) {
		return (~PINF & (1 << (ordinal%8-1)));
	}
	
	return 0;
	
    switch (ordinal) {
        case 1:
           return (~PIND & (1 << 7));
        case 2:
           return (~PIND & (1 << 6));
        case 3:
           return (~PIND & (1 << 5));
        case 4:
           return (~PIND & (1 << 4));
        case 5:
           return (~PIND & (1 << 3));
        case 6:
           return (~PIND & (1 << 2));
        case 7:
           return (~PIND & (1 << 1));
        case 8:
           return (~PIND & (1 << 0)); // not working on prototype

        case 9:
           return (~PINE & (1 << 7));
        case 10:
           return (~PINE & (1 << 6));
        case 11:
           return (~PINE & (1 << 5));
        case 12:
           return (~PINE & (1 << 4));
        case 13:
           return (~PINE & (1 << 3));
        case 14:
           return (~PINE & (1 << 2));
        case 15:
           return (~PINE & (1 << 1));
        case 16:
           return (~PINE & (1 << 0));

        case 17:
           return (~PINF & (1 << 0));
        case 18:
           return (~PINF & (1 << 1));
        case 19:
           return (~PINF & (1 << 2));
        case 20:
           return (~PINF & (1 << 3));
        case 21:
           return (~PINF & (1 << 4));
        case 22:
           return (~PINF & (1 << 5));
        case 23:
           return (~PINF & (1 << 6));
        case 24:
           return (~PINF & (1 << 7));
    }

    return 0;
}

int outputState(uint8_t ordinal) {
    switch (ordinal) {
        case 1:
            return PIN_STATE(PINA, 0);
        case 2:
            return PIN_STATE(PINA, 1);
        case 3:
            return PIN_STATE(PINA, 2);
        case 4:
            return PIN_STATE(PINA, 3);
        case 5:
            return PIN_STATE(PINA, 4);
        case 6:
            return PIN_STATE(PINA, 5);
        case 7:
            return PIN_STATE(PINA, 6);
        case 8:
            return PIN_STATE(PINA, 7);
        
        case 9:
            return PIN_STATE(PING, 2);
        case 10:
            return PIN_STATE(PINJ, 6);
        case 11:
            return PIN_STATE(PINJ, 5);
        case 12:
            return PIN_STATE(PINJ, 4);
        case 13:
            return PIN_STATE(PINJ, 3);
        case 14:
            return PIN_STATE(PINJ, 2);
        case 15:
            return PIN_STATE(PINJ, 1);
        case 16:
            return PIN_STATE(PINJ, 0);
        
        case 17:
            return PIN_STATE(PINC, 7);
        case 18:
            return PIN_STATE(PINC, 6);
        case 19:
            return PIN_STATE(PINC, 5);
        case 20:
            return PIN_STATE(PINC, 4);
        case 21:
            return PIN_STATE(PINC, 3);
        case 22:
            return PIN_STATE(PINC, 2);
        case 23:
            return PIN_STATE(PINC, 1);
        case 24:
            return PIN_STATE(PINC, 0);
    }

    return 0;
}

void output(uint8_t ordinal, uint8_t state) {
    switch (ordinal) {
        case 1:
            if (state) { 
                SET_BIT(PORTA, 0); 
            } else { 
                RESET_BIT(PORTA, 0); 
            } 
            break;
        case 2:                     // not working on prototype
            if (state) { 
                SET_BIT(PORTA, 1);
            } else { 
                RESET_BIT(PORTA, 1);
            }
            break;
        case 3:
            if (state) { 
                SET_BIT(PORTA, 2);
            } else { 
                RESET_BIT(PORTA, 2);
            }
            break;
        case 4:
            if (state) { 
                SET_BIT(PORTA, 3);
            } else { 
                RESET_BIT(PORTA, 3);
            }
            break;
        case 5:
            if (state) { 
                SET_BIT(PORTA, 4);
            } else { 
                RESET_BIT(PORTA, 4);
            }
            break;
        case 6:
            if (state) { 
                SET_BIT(PORTA, 5);
            } else { 
                RESET_BIT(PORTA, 5);
            }
            break;
        case 7:
            if (state) { 
                SET_BIT(PORTA, 6);
            } else { 
                RESET_BIT(PORTA, 6);
            }
            break;
        case 8:
            if (state) { 
                SET_BIT(PORTA, 7);
            } else { 
                RESET_BIT(PORTA, 7);
            }
            break;
        
        case 9:
            if (state) { 
                SET_BIT(PORTG, 2);
            } else { 
                RESET_BIT(PORTG, 2);
            }
            break;
        case 10:
            if (state) { 
                SET_BIT(PORTJ, 6);
            } else { 
                RESET_BIT(PORTJ, 6);
            }
            break;
        case 11:
            if (state) { 
                SET_BIT(PORTJ, 5);
            } else { 
                RESET_BIT(PORTJ, 5);
            }
            break;
        case 12:
            if (state) { 
                SET_BIT(PORTJ, 4);
            } else { 
                RESET_BIT(PORTJ, 4);
            }
            break;
        case 13:
            if (state) { 
                SET_BIT(PORTJ, 3);
            } else { 
                RESET_BIT(PORTJ, 3);
            }
            break;
        case 14:
            if (state) { 
                SET_BIT(PORTJ, 2);
            } else { 
                RESET_BIT(PORTJ, 2);
            }
            break;
        case 15:
            if (state) { 
                SET_BIT(PORTJ, 1);
            } else { 
                RESET_BIT(PORTJ, 1);
            }
            break;
        case 16:
            if (state) { 
                SET_BIT(PORTJ, 0);
            } else { 
                RESET_BIT(PORTJ, 0);
            }
            break;
        
        case 17:                     // not working on prototype
            if (state) { 
                SET_BIT(PORTC, 7);
            } else { 
                RESET_BIT(PORTC, 7);
            }
            break;
        case 18:
            if (state) { 
                SET_BIT(PORTC, 6);
            } else { 
                RESET_BIT(PORTC, 6);
            }
            break;
        case 19:
            if (state) { 
                SET_BIT(PORTC, 5);
            } else { 
                RESET_BIT(PORTC, 5);
            }
            break;
        case 20:
            if (state) { 
                SET_BIT(PORTC, 4);
            } else { 
                RESET_BIT(PORTC, 4);
            }
            break;
        case 21:
            if (state) { 
                SET_BIT(PORTC, 3);
            } else { 
                RESET_BIT(PORTC, 3);
            }
            break;
        case 22:
            if (state) { 
                SET_BIT(PORTC, 2);
            } else { 
                RESET_BIT(PORTC, 2);
            }
            break;
        case 23:
            if (state) { 
                SET_BIT(PORTC, 1);
            } else { 
                RESET_BIT(PORTC, 1);
            }
            break;
        case 24:
            if (state) { 
                SET_BIT(PORTC, 0);
            } else { 
                RESET_BIT(PORTC, 0);
            }
            break;
    }
}

void turnOn(uint8_t out) {
    output(out, 1);
}

void turnOff(uint8_t out) {
    output(out, 0);
}

void toggle(uint8_t out) {
    switch (out) {
        case 1:
            TOGGLE_BIT(PORTA, 0);
            break;
        case 2:
            TOGGLE_BIT(PORTA, 1);
            break;
        case 3:
            TOGGLE_BIT(PORTA, 2);
            break;
        case 4:
            TOGGLE_BIT(PORTA, 3);
            break;
        case 5:
            TOGGLE_BIT(PORTA, 4);
            break;
        case 6:
            TOGGLE_BIT(PORTA, 5);
            break;
        case 7:
            TOGGLE_BIT(PORTA, 6);
            break;
        case 8:
            TOGGLE_BIT(PORTA, 7);
            break;

        case 9:
            TOGGLE_BIT(PORTG, 2);
            break;
        case 10:
            TOGGLE_BIT(PORTJ, 6);
            break;
        case 11:
            TOGGLE_BIT(PORTJ, 5);
            break;
        case 12:
            TOGGLE_BIT(PORTJ, 4);
            break;
        case 13:
            TOGGLE_BIT(PORTJ, 3);
            break;
        case 14:
            TOGGLE_BIT(PORTJ, 2);
            break;
        case 15:
            TOGGLE_BIT(PORTJ, 1);
            break;
        case 16:
            TOGGLE_BIT(PORTJ, 0);
            break;

        case 17:
            TOGGLE_BIT(PORTC, 7);
            break;
        case 18:
            TOGGLE_BIT(PORTC, 6);
            break;
        case 19:
            TOGGLE_BIT(PORTC, 5);
            break;
        case 20:
            TOGGLE_BIT(PORTC, 4);
            break;
        case 21:
            TOGGLE_BIT(PORTC, 3);
            break;
        case 22:
            TOGGLE_BIT(PORTC, 2);
            break;
        case 23:
            TOGGLE_BIT(PORTC, 1);
            break;
        case 24:
            TOGGLE_BIT(PORTC, 0);
            break;
    }
}

bool pressed(uint8_t inputNumber, uint8_t delayMode) {
    static bool wasToggled[INPUTS_COUNT] = { false };
    static bool wasPressed[INPUTS_COUNT] = { false };
    static bool pressedShort[INPUTS_COUNT] = { false };
    static bool pressedLong[INPUTS_COUNT] = { false };
    static uint32_t stopTime[INPUTS_COUNT] = { 0 };
    static uint32_t startTime[INPUTS_COUNT] = { 0 };

    bool result = false;

    if (input(inputNumber)) {
        if (wasPressed[inputNumber]) {
            stopTime[inputNumber] = millis();
            pressedLong[inputNumber] = (stopTime[inputNumber] - startTime[inputNumber]) > longDelay[inputNumber];
        } else {
            wasPressed[inputNumber] = true;
            startTime[inputNumber] = millis();        
        }
    } else {
        if (wasPressed[inputNumber]) {
            wasToggled[inputNumber] = true;
        }

        wasPressed[inputNumber] = false;
    }

    if (wasToggled[inputNumber]) {
        uint32_t lastDuration = stopTime[inputNumber] - startTime[inputNumber];
         
        pressedLong[inputNumber] = (lastDuration >= longDelay[inputNumber]);
        pressedShort[inputNumber] = (!pressedLong[inputNumber] && (lastDuration >= shortDelay[inputNumber]));
        
        wasToggled[inputNumber] = false;
    }

    if (delayMode == DELAY_SHORT) {
        result = pressedShort[inputNumber];
        pressedShort[inputNumber] = false;
    } else if (delayMode == DELAY_LONG) {
        result = pressedLong[inputNumber];
        pressedLong[inputNumber] = false;
    }

    return result;
}

bool pressed(uint8_t inputNumber) {
    return pressed(inputNumber, DELAY_SHORT);
}

#define MINIMAL_HOLD_TIME 50

bool held(uint8_t inputNumber) {
    static bool wasPressed[INPUTS_COUNT] = { false };    
    static uint32_t startTime[INPUTS_COUNT] = { 0 };

    if (input(inputNumber)) {
        if (wasPressed[inputNumber]) {
            if (millis() - startTime[inputNumber] > MINIMAL_HOLD_TIME) {
                return true;
            }
        } else {
            wasPressed[inputNumber] = true;
            startTime[inputNumber] = millis();        
        }
    } else {
        wasPressed[inputNumber] = false;
    }

    return false;
}
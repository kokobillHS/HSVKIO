/*
 * time.cpp
 *
 * Created: 05.09.2016 16:27:12
 * Author: dmitry
 */ 

#include "include/main.h"

volatile uint32_t currentMillisInt = 0;
volatile uint8_t currentMillisFrac = 0;


void initTimer() {
    TCCR2B = (1 << CS21);// | (1 << CS20);
    TIFR2 = (1 << TOV2);
    TIMSK2 = (1 << TOIE2);
}

uint32_t millis() {
    uint32_t result;

    cli();        
        result = currentMillisInt;
    sei();

    return result;
}

ISR(TIMER2_OVF_vect) {
    // copy these to local variables so they can be stored in registers
    // (volatile variables must be read from memory on every access)
    uint32_t millisInt = currentMillisInt;
    uint8_t millisFrac = currentMillisFrac;
    
    millisInt += MILLIS_INC;
    millisFrac += FRAC_INC;

    if (millisFrac >= FRAC_MAX) {
        millisFrac -= FRAC_MAX;
        millisInt += 1;
    }
    
    currentMillisInt = millisInt;
    currentMillisFrac = millisFrac;
}
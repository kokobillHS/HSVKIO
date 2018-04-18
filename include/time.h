/*
 * time.h
 *
 * Created: 05.09.2016 12:29:52
 *  Author: dmitry
 */ 


#ifndef TIME_H_
#define TIME_H_

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/interrupt.h>

#define cyclesPerMicrosecond() ( F_CPU / 1000000L )
#define cyclesToMicroseconds(c) ( ((c) * 1000L) / (F_CPU / 1000L) )
#define TIMER2_PRESCALER 8
#define MICROSECONDS_PER_TIMER_OVERFLOW (cyclesToMicroseconds(TIMER2_PRESCALER * 256))
// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER_OVERFLOW / 1000)
// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRAC_INC ((MICROSECONDS_PER_TIMER_OVERFLOW % 1000) >> 3)
#define FRAC_MAX (1000 >> 3)

void initTimer();

uint32_t millis();

#endif /* TIME_H_ */
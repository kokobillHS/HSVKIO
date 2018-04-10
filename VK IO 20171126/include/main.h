/*
 * main.h
 *
 * Created: 28.07.2016 17:58:30
 *  Author: dmitry
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//#ifndef F_OSC
#define F_OSC 8000000UL
//#endif

#define F_CPU 8000000UL

#define AWAY_DELAY_TIME 5000L

#include "pindefs.h"
#include "helpers.h"
#include "digitalio.h"
#include "time.h"


void initPorts();
void processIO();
void processSpecialModes();

#endif /* MAIN_H_ */
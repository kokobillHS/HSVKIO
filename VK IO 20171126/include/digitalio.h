/*
 * digitalio.h
 *
 * Created: 18.08.2016 16:26:57
 *  Author: dmitry
 */ 


#ifndef DIGITALIO_H_
#define DIGITALIO_H_

#define INPUTS_COUNT 24
#define OUTPUTS_COUNT 24

#define DELAY_LONG 1
#define DELAY_SHORT 0

int input(uint8_t ordinal);
int outputState(uint8_t ordinal);

void output(uint8_t ordinal, uint8_t state);
void turnOn(uint8_t out);
void turnOff(uint8_t out);
void toggle(uint8_t out);

bool pressed(uint8_t inputNumber, uint8_t delayMode);
bool pressed(uint8_t inputNumber);
bool held(uint8_t inputNumber);

#endif /* DIGITALIO_H_ */
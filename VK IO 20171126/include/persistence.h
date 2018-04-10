/*
 * persistence.h
 *
 * Created: 18.11.2017 15:24:57
 *  Author: dmitry
 */ 


#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include <avr/eeprom.h>

#define BYTES_PER_MODE 24 // maximum count of outputs
#define MEM_OFFSET(md) ((md) * BYTES_PER_MODE)

void initEEPROM();
void setStartingAddress(uint16_t addr);
uint16_t getStartingAddress();

void readSettings(uint8_t *dest, uint8_t devCount, uint8_t modeCode);
void writeSettings(const uint8_t *settings, uint8_t devCount, uint8_t modeCode);

void updateSingleSetting(uint8_t index, uint8_t setting, uint8_t modeCode);
uint8_t readCurrentMode();
void writeCurrentMode(uint8_t value);

#endif /* PERSISTENCE_H_ */
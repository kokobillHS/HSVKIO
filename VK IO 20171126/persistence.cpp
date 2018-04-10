/*
 * persistence.cpp
 *
 * Created: 18.11.2017 15:26:50
 *  Author: dmitry
 */ 

#include "include/persistence.h"
#include "include/apartment.h"

static uint16_t startingAddress = 0x00;
static const uint32_t magicBytes = 0xBABECAFEUL;

void initEEPROM() {
    uint32_t currentSignature = 0;
    currentSignature = eeprom_read_dword((uint32_t *)startingAddress);

    if (currentSignature == magicBytes) {
        readSettingsFromStorage();
    } else {
        eeprom_write_dword((uint32_t *)startingAddress, magicBytes);
        writeSettingsToStorage();
    }
}

void setStartingAddress(uint16_t addr) {
    startingAddress = addr;
}

uint16_t getStartingAddress() {
    return startingAddress;
}

void readSettings(uint8_t *dest, uint8_t devCount, uint8_t modeCode) {
    uint8_t *eepromAddr = (uint8_t *)startingAddress + sizeof(magicBytes) + MEM_OFFSET(modeCode);    
    eeprom_read_block(dest, eepromAddr, devCount);
}

void writeSettings(const uint8_t *settings, uint8_t devCount, uint8_t modeCode) {
    uint8_t *eepromAddr = (uint8_t *)startingAddress + sizeof(magicBytes) + MEM_OFFSET(modeCode);
    eeprom_update_block(settings, eepromAddr, devCount);
}

void updateSingleSetting(uint8_t index, uint8_t setting, uint8_t modeCode) {
    uint8_t *eepromAddr = (uint8_t *)startingAddress + sizeof(magicBytes) + MEM_OFFSET(modeCode) + index;
    eeprom_update_byte(eepromAddr, setting);
}

uint8_t readCurrentMode() {
    return eeprom_read_byte((uint8_t *)startingAddress + sizeof(magicBytes) + MEM_OFFSET(MODE_COUNT)) & 0b11;
}

void writeCurrentMode(uint8_t value) {
    eeprom_update_byte((uint8_t *)startingAddress + sizeof(magicBytes) + MEM_OFFSET(MODE_COUNT), value);
}
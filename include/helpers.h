/*
 * helpers.h
 *
 * Created: 02.09.2016 12:46:57
 *  Author: dmitry
 */ 


#ifndef HELPERS_H_
#define HELPERS_H_

#define TOGGLE_BIT(word, bit) word ^= (1 << (bit))
#define SET_BIT(word, bit) word |= (1 << (bit))
#define RESET_BIT(word, bit) word &= ~(1 << (bit))
#define PIN_STATE(port, pin) (!!((port) & (1 << (pin))))


#endif /* HELPERS_H_ */
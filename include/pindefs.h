/*
 * pindefs.h
 *
 * Created: 28.07.2016 16:41:57
 *  Author: dmitry
 */ 


#ifndef PINDEFS_H_
#define PINDEFS_H_

#define DATA_REGISTER_EMPTY (1<<UDRE0)
#define RX_COMPLETE (1<<RXC0)
#define FRAMING_ERROR (1<<FE0)
#define PARITY_ERROR (1<<UPE0)
#define DATA_OVERRUN (1<<DOR0)


#define LED_1_ON  PORTL |=(1<<0)        // CPU run
#define LED_1_OFF PORTL &=~(1<<0)
#define LED_1_TOGGLE PORTL ^= (1 << 0)

#define LED_2_ON  PORTL |=(1<<1)        // Error 1
#define LED_2_OFF PORTL &=~(1<<1)

#define LED_3_ON  PORTL |=(1<<2)        // Program mode
#define LED_3_OFF PORTL &=~(1<<2)

#define LED_4_ON  PORTL |=(1<<3)        //  Info 1
#define LED_4_OFF PORTL &=~(1<<3)

#define LED_5_ON  PORTL |=(1<<4)        // Info 2
#define LED_5_OFF PORTL &=~(1<<4)
#define LED_5_TOGGLE PORTL ^= (1 << 4)

#define IN_1   ~PIND & (1<<7)
#define IN_2   ~PIND & (1<<6)
#define IN_3   ~PIND & (1<<5)
#define IN_4   ~PIND & (1<<4)
#define IN_5   ~PIND & (1<<3)
#define IN_6   ~PIND & (1<<2)
#define IN_7   ~PIND & (1<<1)
#define IN_8   ~PIND & (1<<0)

#define IN_9    ~PINE & (1<<7)
#define IN_10   ~PINE & (1<<6)
#define IN_11   ~PINE & (1<<5)
#define IN_12   ~PINE & (1<<4)
#define IN_13   ~PINE & (1<<3)
#define IN_14   ~PINE & (1<<2)
#define IN_15   ~PINE & (1<<1)
#define IN_16   ~PINE & (1<<0)

#define IN_17   ~PINF & (1<<0)
#define IN_18   ~PINF & (1<<1)
#define IN_19   ~PINF & (1<<2)
#define IN_20   ~PINF & (1<<3)
#define IN_21   ~PINF & (1<<4)
#define IN_22   ~PINF & (1<<5)
#define IN_23   ~PINF & (1<<6)
#define IN_24   ~PINF & (1<<7)


#endif /* PINDEFS_H_ */
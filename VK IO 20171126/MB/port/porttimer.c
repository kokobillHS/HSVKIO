/*
 * FreeModbus Libary: ATMega168 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 * Modified in 2010 by drvlas: 
 *	Only RTU mode. 
 *	Use T0 instead of T1. Implemented only on ATmega168
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.4 2006/09/03 11:53:10 wolti Exp $
 */

/* ----------------------- AVR includes -------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

/* ----------------------- Platform includes --------------------------------*/
#include "../include/port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "../include/mb.h"
#include "../include/mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_TIMER_PRESCALER      ( 1024UL )
#define MB_TIMER_TICKS          ( F_CPU / MB_TIMER_PRESCALER )
#define MB_50US_TICKS           ( 20000UL )

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usTimerOCRADelta;
//static USHORT   usTimerOCRBDelta;

/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTimerout50us )
{
    /* Calculate overflow counter an OCR values for Timer0. */
    usTimerOCRADelta =
        ( MB_TIMER_TICKS * usTimerout50us ) / ( MB_50US_TICKS );
	if( usTimerOCRADelta > 255) return FALSE;
    TCCR0A = 0x00;		// Normal Mode + Output Compare interrupt
    TCCR0B = 0x00;
//    TCCR1C = 0x00;

    vMBPortTimersDisable(  );

    return TRUE;
}


inline void
vMBPortTimersEnable(  )
{
    TCNT0 = 0x0000;
    if( usTimerOCRADelta > 0 )
    {
        TIMSK0 |= _BV( OCIE0A );
        OCR0A = usTimerOCRADelta;	// MUST be < 256
    }

    TCCR0B |= _BV( CS12 ) | _BV( CS10 );	// Fosc/1024
}

inline void
vMBPortTimersDisable(  )
{
    /* Disable the timer. */
    TCCR0B &= ~( _BV( CS12 ) | _BV( CS10 ) );
    /* Disable the output compare interrupts for channel A */
    TIMSK0 &= ~( _BV( OCIE0A ) );
    /* Clear output compare flags for channel A/B. */
    TIFR0 |= _BV( OCF0A ) ;
}

ISR(TIMER0_COMPA_vect)
{
    ( void )pxMBPortCBTimerExpired(  );
}


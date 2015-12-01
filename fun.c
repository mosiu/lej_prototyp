/*
 * fun.c
 *
 * Created: 11/1/2015 8:51:50 PM
 *  Author: mosiu
 */ 


#include "LCD.h"
#include "button.h"
#include "defines.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void TCCR2_start(void);
void TCCR2_stop(void);

// prescaler to 64 + OCR2 to 155 = 10ms Output Compare Interrupt 2 period
const uint8_t TCCR2_presc_mask = _BV(CS22);

static uint16_t TimeCounter = 0u;

static uint8_t CountingActivated = 0u;

void init_timer (void)
{
	sei();
	// set Clear Timer on Compare mode
	TCCR2 &= ~(_BV(WGM21)|_BV(WGM20));
	TCCR2 |= _BV(WGM21);

	TCCR2_start();
	
	OCR2 = (uint8_t)155u;
	TimeCounter = 0u;
	// interrupt set and timer started in start_timer()
	return;
}


// TODO:: this is a ISR! display function call not allowed here. Write interface
// to LCD allowing time increment flag set - an increment operation would be executed
// from main, or something like that.
ISR ( TIMER2_COMP_vect, ISR_BLOCK )
{
	if (CountingActivated)
	{
		TimeCounter ++;
	}
	STM_tick();
	button_tick();
	return;
}


uint16_t* get_current_time(void)
{
	return &TimeCounter;
};

void start_time_measurement(void)
{
	TimeCounter = 0u;
	CountingActivated = 1u;
}

void stop_time_measurement(void)
{
	CountingActivated = 0u;
}

void TCCR2_start(void)
{
	// clear prescaler setting
	TCCR2 &= ~(_BV(CS22)|_BV(CS21)|_BV(CS20));
	// start counting
	TCCR2 |= TCCR2_presc_mask;
	// enable interrupt
	TIMSK |= _BV(OCIE2);
}

void TCCR2_stop(void)
{
	// prescaler to 64 + OCR2 to 155 = 10ms Output Compare Interrupt 2 period
	TCCR2 &= ~(_BV(CS22)|_BV(CS21)|_BV(CS20));
	// disable interrupt
	TIMSK &= ~_BV(OCIE2);
}

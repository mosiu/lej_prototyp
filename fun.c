/*
 * fun.c
 *
 * Created: 11/1/2015 8:51:50 PM
 *  Author: mosiu
 */ 


#include "LCD.h"
#include "defines.h"
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL //stala doslowna, unsigned long, jak w Greboszu
#include <util/delay.h>
/*
typedef struct {
	uint8_t line1;
	uint8_t line2;
	uint8_t line3;
	uint8_t line4;
	uint8_t line5;
	uint8_t line6;
	uint8_t line7;
	uint8_t line8;
} cgram_t;

typedef union {
	cgram_t cgram_output;
	uint64_t flash_input;
	} character_t;

typedef struct {
	character_t a;
	character_t b;
	character_t c;
	character_t d;
} large_digit_t;
*/

// 
static uint16_t timeCounter;


void init_systick (void)
{
	sei();
	// set Clear Timer on Compare mode
	TCCR2 &= ~(_BV(WGM21)|_BV(WGM20));
	TCCR2 |= _BV(WGM21);
	// interrupt enable
	//TIMSK |= _BV(TOIE2);
	TIMSK |= _BV(OCIE2);
	// prescaler to 64 + OCR2 to 155 = 10ms Output Compare Interrupt 2 period
	TCCR2 &= ~(_BV(CS22)|_BV(CS21)|_BV(CS20));
	TCCR2 |= _BV(CS22);	
	OCR2 = (uint8_t)155u;
	return;
}


// TODO:: this is a ISR! display function call not allowed here. Write interface
// to LCD allowing time increment flag set - an increment operation would be executed
// from main, or something like that.
ISR ( TIMER2_COMP_vect, ISR_BLOCK )
{
	
	timeCounter++;
	if (!(timeCounter%10))
		display_time(&timeCounter);
	return;
}

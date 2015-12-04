/*
 * CFile1.c
 *
 * Created: 11/27/2015 7:04:00 PM
 *  Author: mosiu
 */ 

#include "button.h"
#include "LCD.h"
#include "STM.h"
#include "fun.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include <util/delay.h>

#define BUTTON_PORT PORTC
#define BUTTON_PIN	PINC
#define BUTTON_DDR	DDRC
#define BUTTON_MASK	(1<<PC4)

static uint8_t DebounceCounter = 0u;
static uint8_t IfButtonReallyPressed = 0u;
static uint8_t IfButtonReleased = 1u;

/*
Returns logical high if the button is pressed.
*/
uint8_t if_button_pressed(void);

void button_action();

void button_refresh(void)
{
	if (IfButtonReallyPressed)
	{
		button_action();
		IfButtonReallyPressed = 0u;
	}
}

void button_init(void)
{
	// to input
	BUTTON_DDR &= ~BUTTON_MASK;
	
	// to high with pull-up
	BUTTON_PORT |= BUTTON_MASK;
}

void button_tick(void)
{
	if (IfButtonReleased)
	{
		// FIRST TIME DETECTION
		if(if_button_pressed() && !(DebounceCounter))
		{
			DebounceCounter++;
			return;
		}
		
		// FURTHER DETECTION
		// if push already detected (DebounceCounter) and button still pressed (button_poll())
		if(if_button_pressed() && DebounceCounter)
		{
			// increment counter
			DebounceCounter++;
		}
		else
		{
			// if counting not started and/or button not pressed anymore,
			DebounceCounter = 0u;
		}
		
		// if the button is held long enough
		if (DebounceCounter == 4u)
		{
			IfButtonReallyPressed = 1u;
			IfButtonReleased = 0u;
		}
		
	}
	else	// if button pressed, until release
	{
		// wait for button release
		if (!if_button_pressed())
		{
			IfButtonReleased = 1u;	// mark release occurence
		}
	}

	
}

void button_action()
{
	STM_switch_superstate();
}


uint8_t if_button_pressed(void)
{
	return !(BUTTON_PIN & BUTTON_MASK);
}
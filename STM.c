/*
 * STM.c
 *
 * Created: 11/29/2015 6:09:59 PM
 *  Author: mosiu
 */ 
#include "ADC.h"
#include "STM.h"
#include "LCD.h"
#include "fun.h"

#define REED_PORT	PORTD
#define REED_PIN	PIND
#define REED_DDR	DDRD
#define REED_MASK	(1 << PD3)

#define ADC_LOW_SWITCH_VALUE	20u
#define ADC_HIGH_SWITCH_VALUE	130u

static uint8_t if_ready_entry = 1u; // IMPORTANT!!
static uint8_t if_adc_entry = 0u;
static uint8_t if_drinking_entry = 0u;
static uint8_t if_display_entry = 0u;

static uint8_t If0_01secPassed = 0u;
static uint8_t If0_1secPassed = 0u;

state_t AppState = READY_STATE;

uint8_t if_valve_open(void);

void STM_switch_superstate(void)
{
	static state_t substate;
	if (AppState == ADC_STATE)
	{
		// no entry flag, the previous state is just continued, 
		// LCD content remains obsolete ADC printout until next STM_set_state
		AppState = substate;
	}
	else
	{
		if_adc_entry = 1u;
		
		substate = AppState; // remember previous state
		AppState = ADC_STATE;
	}
}

void STM_tick(void)
{
	static uint8_t tick_counter = 0u;
	If0_01secPassed = 1u;
	tick_counter ++;
	if (tick_counter == 10)
	{
		tick_counter = 0;
		If0_1secPassed = 1u;
	}
}

state_t STM_get_state(void)
{
	return AppState;
}

// the entry cannot be executed from main, because sometime i might want to 
// switch state in an interrupt. Interrupt should never take long to execute.
// That's why it only calls start/stop measurement function (time-critical).
// LCD operations need to be executed in main - that is, in STM_refresh.
void STM_set_state(state_t state)
{
	switch(state)
	{
	case READY_STATE:
		if_ready_entry = 1u;
		break;
	case DRINKING_STATE:
		start_time_measurement();
		if_drinking_entry = 1u;
		break;
	case DISPLAY_STATE:
		stop_time_measurement();
		if_display_entry = 1u;
		break;
	case ADC_STATE:
		if_adc_entry = 1u;
		break;
	}
	AppState = state;
}

void STM_refresh(void)
{
	switch(AppState)
	{
	case READY_STATE:
		if (if_ready_entry)
		{
			if_ready_entry = 0u;
			LCD_clear();
			LCD_putsub("Wez go \ndo buzi");
		}
		else
		{
			if (if_valve_open())
			{
				STM_set_state(DRINKING_STATE);
			}
		}
		break;
	case DRINKING_STATE:
		if (if_drinking_entry)
		{
			LCD_clear();
			if_drinking_entry = 0u;
			// place remaining entry code here
		}
		else
		{
			if (ADC_get_result() < ADC_LOW_SWITCH_VALUE)
			{
				STM_set_state(DISPLAY_STATE);
			}
			// DRINKING_STATE NORMAL OPERATION
			if (If0_01secPassed)
			{
				LCD_display_time(get_current_time());
				If0_01secPassed = 0u;
			}
		}
		break;
	case DISPLAY_STATE:
		if (if_display_entry)
		{
			LCD_display_time(get_current_time()); // drinking time end display
			if_display_entry = 0u;
		}
		else
		{
			// poll the ADC (wait for the beer)
			if ((ADC_get_result() > ADC_HIGH_SWITCH_VALUE) && ! if_valve_open())
			{
				STM_set_state(READY_STATE);
			}
		}
		break;
	case ADC_STATE:
		if (if_adc_entry)
		{
			LCD_clear();
			if_adc_entry = 0u;
		}
		else
		{
			// ADC_STATE NORMAL OPERATION
			if (If0_01secPassed)
			{
				LCD_carriage_return();
				LCD_putsub("   ");
				LCD_carriage_return();
				LCD_display_number((uint16_t)ADC_get_result());
				If0_01secPassed = 0u;
			}
		}
		break;
	}
}

void reed_init(void)
{
	// ddr to input
	REED_DDR &= ~REED_MASK;
	// to high via pull-up
	REED_PORT |= REED_MASK; 
}

uint8_t if_valve_open(void)
{
	return !(REED_PIN & REED_MASK);
}
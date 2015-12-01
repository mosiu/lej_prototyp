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

static uint8_t if_tc_entry = 1u; // IMPORTANT!!
static uint8_t if_adc_entry = 0u;

static uint8_t If0_01secPassed = 0u;
static uint8_t If0_1secPassed = 0u;

state_t AppState = READY;

void STM_switch_superstate(void)
{
	if (AppState == ADC_STATE)
	{
		if_tc_entry = 1u;
		AppState = READY;
	}
	else
	{
		if_adc_entry = 1u;
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

void STM_refresh(void)
{
	switch(AppState)
	{
	case READY:
		if (if_tc_entry)
		{
			LCD_clear();
			start_time_measurement();
			if_tc_entry = 0u;
		}
		else
		{
			// READY_STATE NORMAL OPERATION
			if(If0_01secPassed)
			{
				LCD_display_time(get_current_time());
				// clear time increment flag
				If0_01secPassed = 0u;
			}
		}
		break;
	case ADC_STATE:
		if (if_adc_entry)
		{
			LCD_clear();
			stop_time_measurement();
			if_adc_entry = 0u;
		}
		else
		{
			// ADC_STATE NORMAL OPERATION
			if (If0_1secPassed)
			{
				LCD_clear();
				LCD_display_number((uint16_t)ADC_get_result());
				If0_1secPassed = 0u;
			}
		}
		break;
	}
}
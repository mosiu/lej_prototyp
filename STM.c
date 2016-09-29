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
#define REED_MASK	(1 << PD2)

#define LED_PORT	PORTC
#define LED_DDR		DDRC
#define LED_MASK	(1 << PC5)

#define ADC_LOW_SWITCH_VALUE	19u
#define ADC_HIGH_SWITCH_VALUE	130u

#define LED_BLINK_LENGTH		100u		// measured in 1/100 sec

#define VERIFICATION_LENGTH		80u

static uint8_t state_is_new = 1u;

static uint8_t verification_state_duration = 0u;
static uint8_t led_flashing_duration = 0u;


static uint8_t If0_01secPassed = 0u;

static uint16_t verified_time = 0u;

static state_t AppState = READY_STATE;

uint8_t if_valve_open(void);
void clear_flags(void);
void execute_state_entry(state_t state);

void led_on();
void led_off();

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
		substate = AppState; // remember previous state
		AppState = ADC_STATE;
	}
}

void STM_tick(void)
{
	If0_01secPassed = 1u;
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
	state_is_new = 1u;
	AppState = state;
}

void execute_state_entry(state_t state)
{
	switch(state)
	{

		case READY_STATE:
			LCD_clear();
			LCD_putsub("Wez go \ndo buzi");
			break;
		case DRINKING_STATE:
			led_on();
			LCD_clear();
			start_time_measurement();
			break;
		case VERIFICATION_STATE:
			verified_time = *get_current_time();
			LCD_display_time(&verified_time);
			verification_state_duration = 0u;
			break;
		case DISPLAY_STATE:
			stop_time_measurement();
			//led_on();
			led_flashing_duration = 0u;
			//LCD_display_time(get_current_time()); // drinking time end display
			break;
		case ADC_STATE:
			LCD_clear();
			break;

	}
	state_is_new = 0u;
}

void STM_refresh(void)
{
	if(state_is_new)
	{
		execute_state_entry(AppState);
	}
	

	switch(AppState)
	{
	case READY_STATE:
		if (if_valve_open())
		{
			STM_set_state(DRINKING_STATE);
		}
		break;
	case DRINKING_STATE:

		if (ADC_get_result() < ADC_LOW_SWITCH_VALUE)
		{
			STM_set_state(VERIFICATION_STATE);
		}
		if (If0_01secPassed)
		{
			LCD_display_time(get_current_time());
		}
		break;
		
	case VERIFICATION_STATE:
		if(If0_01secPassed)
		{
			verification_state_duration ++;
		}
		if(ADC_get_result() > ADC_LOW_SWITCH_VALUE)
		{
			AppState = DRINKING_STATE;	// don't execute drinking_state entry!

		}
		if(verification_state_duration >= VERIFICATION_LENGTH)
		{
			STM_set_state(DISPLAY_STATE);
		}
		break;
	case DISPLAY_STATE:
		if (If0_01secPassed)
		{
			led_flashing_duration ++;
			if (led_flashing_duration >= LED_BLINK_LENGTH)
			{
				led_off();
			}
		}
		
		// poll the ADC (wait for the beer)
		if ((ADC_get_result() > ADC_HIGH_SWITCH_VALUE) && ! if_valve_open())
		{
			STM_set_state(READY_STATE);
		}
		break;
	case ADC_STATE:		
		// ADC_STATE NORMAL OPERATION
		if (If0_01secPassed)
		{
			LCD_carriage_return();
			LCD_putsub("   ");
			LCD_carriage_return();
			LCD_display_number((uint16_t)ADC_get_result());
		}
		break;
	}
	
	clear_flags();
}

void clear_flags(void)
{
	If0_01secPassed = 0u;
}

void reed_init(void)
{
	// ddr to input
	REED_DDR &= ~REED_MASK;
	// to high via pull-up
	REED_PORT |= REED_MASK;
}

void led_init()
{
	LED_DDR |= LED_MASK;
	LED_PORT |= LED_MASK;
}

void led_on()
{
	LED_PORT &= ~LED_MASK;
}

void led_off()
{
	LED_PORT |= LED_MASK;
}

uint8_t if_valve_open(void)
{
	return !(REED_PIN & REED_MASK);
}
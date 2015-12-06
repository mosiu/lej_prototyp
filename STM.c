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

#define LED_PORT	PORTC
#define LED_DDR		DDRC
#define LED_MASK	(1 << PC3)

#define ADC_LOW_SWITCH_VALUE	16u
#define ADC_HIGH_SWITCH_VALUE	130u
#define VERIFICATION_STATE_DURATION		7u

#define LED_BLINK_LENGTH		100u		// measured in 1/100 sec

static uint8_t if_ready_entry = 1u; // IMPORTANT!!
static uint8_t if_adc_entry = 0u;
static uint8_t if_drinking_entry = 0u;
static uint8_t if_display_entry = 0u;
static uint8_t if_verification_entry = 0u;

static uint8_t If0_01secPassed = 0u;
static uint8_t If0_1secPassed = 0u;

state_t AppState = READY_STATE;

static uint16_t VerifiedTime;

uint8_t if_valve_open(void);

void led_on();
void led_off();

void STM_switch_superstate(void)
{
	static state_t substate = READY_STATE;
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
	case VERIFICATION_STATE:
		if_verification_entry = 1u;
		VerifiedTime = *get_current_time();
		break;
	case ADC_STATE:
		if_adc_entry = 1u;
		break;
	}
	AppState = state;
}

void STM_refresh(void)
{
	static uint8_t led_counter;
	static uint8_t verification_counter;
	switch(AppState)
	{
	case READY_STATE:
		if (if_ready_entry)
		{
			if_ready_entry = 0u;
			LCD_clear();
			LCD_putsub("Wez go \ndo buzi");
		}
		if (if_valve_open())
		{
			STM_set_state(DRINKING_STATE);
		}
		break;
	case DRINKING_STATE:
		if (if_drinking_entry)
		{
			LCD_clear();
			if_drinking_entry = 0u;
		//	led_off();
			// place remaining entry code here
		}
		if (ADC_get_result() < ADC_LOW_SWITCH_VALUE)
		{
			STM_set_state(VERIFICATION_STATE);
		}
		// DRINKING_STATE NORMAL OPERATION
		if (If0_01secPassed)
		{
			LCD_display_time(get_current_time());
			If0_01secPassed = 0u;
		}
		break;
	case VERIFICATION_STATE:
		if (if_verification_entry)	// start counting
		{
			verification_counter = 0u;
			if_verification_entry = 0u;
		}
		if(If0_1secPassed)		// increment every 0,01 sec
		{
			verification_counter ++;
			If0_1secPassed = 0u;
		}
		if (ADC_get_result() > ADC_LOW_SWITCH_VALUE)	// if the pressure doesn't remain low, continue counting
		{
			STM_set_state(DRINKING_STATE);
		}
		if(verification_counter >= VERIFICATION_STATE_DURATION)	// if after some time nothing happens, display verified value
		{
			STM_set_state(DISPLAY_STATE);
		}
		break;
	case DISPLAY_STATE:
		if (if_display_entry)
		{
			led_on();
			led_counter = 0u;
			LCD_display_time(&VerifiedTime); // drinking time end display
			if_display_entry = 0u;
		}
		if (If0_01secPassed)
		{
			led_counter ++;
			if (led_counter >= LED_BLINK_LENGTH)
			{
				led_off();
			}
			If0_01secPassed = 0u;
		}
		
		// poll the ADC (wait for the beer)
		if ((ADC_get_result() > ADC_HIGH_SWITCH_VALUE) && ! if_valve_open())
		{
			STM_set_state(READY_STATE);
		}
		break;
	case ADC_STATE:
		if (if_adc_entry)
		{
			LCD_clear();
			if_adc_entry = 0u;
		}
		
		// ADC_STATE NORMAL OPERATION
		if (If0_01secPassed)
		{
			LCD_carriage_return();
			LCD_putsub("   ");
			LCD_carriage_return();
			LCD_display_number((uint16_t)ADC_get_result());
			If0_01secPassed = 0u;
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
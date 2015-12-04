/*
 * Prototype.c
 *
 * Created: 11/1/2015 12:32:14 PM
 * Author : mosiu
 */ 
#include "ADC.h"
#include "STM.h"
#include "LCD.h"
#include "defines.h"
#include <util/delay.h>
#include "button.h"
#include "fun.h"

int main(void)
{
	// inicjalizuj LCD
	LCD_init();
	
	// wlacz Timer 2
	init_timer();
	
	// wlacz przycisk
	button_init();
	
	ADC_init();
	
	reed_init();
	
	led_init();
	
	STM_set_state(READY_STATE);
	
    /* Replace with your application code */
    while (1) 
    {
		STM_refresh();
		button_refresh();
    }
}

/*
 * Prototype.c
 *
 * Created: 11/1/2015 12:32:14 PM
 * Author : mosiu
 */ 
#include "LCD.h"
#include "defines.h"
#include <util/delay.h>
#include "button.h"
#include "fun.h"
void led_init();

int main(void)
{
	// inicjalizuj LCD
	LCD_init();
	
	// wlacz Timer 2
	init_timer();
	
	// wlacz przycisk
	button_init();
	
    /* Replace with your application code */
    while (1) 
    {
		LCD_refresh();
		button_refresh();
    }
}

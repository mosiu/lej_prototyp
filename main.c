/*
 * Prototype.c
 *
 * Created: 11/1/2015 12:32:14 PM
 * Author : mosiu
 */ 
#include "LCD.h"
#include "defines.h"

//volatile uint8_t adres = 0;
char linia1[17] = {"f [Hz]:"};
char linia2[17] = {"k [%]:"};



	
void led_init();

int main(void)
{
	// inicjalizuj LCD
	init_lcd();
		
	//LCD wyslij napis
	
	//lcd_disp_number();
	/*	
	SET_ADDR(0x40)
	lcd_putsub(linia2);
	SET_ADDR(0)
	lcd_putsub(linia1);
	*/
	// wlacz przerwania
	led_init();
	// wlacz Timer 2
	init_systick();
	
	
    /* Replace with your application code */
    while (1) 
    {
		//LED_PORT ^= _BV(LED_PIN);
		//_delay_ms(500);

    }
}

void led_init()
{
	// set PC0 as output
	LED_DDR |= (1<<LED_PIN);
}

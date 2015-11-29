#ifndef LCD_H_
#define LCD_H_

#include "hd44780.h"

//********************** M A K R A *******************************

// LCD
					


typedef enum {
	ADC_STATE,
	TIME_COUNTER_STATE
	} state_t;

//********************** F U N K C J E ***********************

void LCD_clear(void);

void LCD_newline(void);

/*
Inicjalizuje LCD
*/
void LCD_init(void);
/*
Wysyla napis zawarty w C-stringu do LCD, '\n' zmienia linie, '\r' maze aktualna i wraca do pocz.
*/  
void LCD_putsub(char* sub);

/*
Wyswietla liczbe 0 - 65535
*/
void  LCD_display_number(uint16_t liczba);

/*
To be called from SysTick ISR.
*/
void LCD_tick(void);

/*
To be called from main. Responsible for changing displayed time every 0.1s
*/
void LCD_refresh(void);

/*
Switches application state.
*/
void LCD_switch_state();


state_t LCD_get_state(void);

#endif // LCD_H_
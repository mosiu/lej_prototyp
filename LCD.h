#ifndef LCD_H_
#define LCD_H_

#include "hd44780.h"

//********************** M A K R A *******************************

// LCD

#define CLEAR_LCD	hd44780_outcmd(HD44780_CLR); \
					hd44780_wait_ready(1); \
					adres = 0;
					
#define SEND_DATA(iks)	hd44780_outdata((iks)); \
						hd44780_wait_ready(0); \
						adres++;
						
#define SET_ADDR(iks)	hd44780_outcmd(HD44780_DDADDR((iks))); \
						hd44780_wait_ready(0); \
						adres = (iks);

#define REPRINT_LN 	lcd_putsub(0); \
					lcd_putsub( flg.czy_wypelnienie ? linia2 : linia1 );

// klawiatura
#define STATIC_RESET	input = 0; 


//extern volatile uint8_t adres;	//zawsze adres pierwszego wolnego znaku LCD

//********************** F U N K C J E ***********************
/*
Inicjalizuje LCD
*/
void init_lcd(void);
/*
Wysyla napis zawarty w C-stringu do LCD, '\n' zmienia linie, '\r' maze aktualna i wraca do pocz.
*/  
void lcd_putsub(char* sub);

/*
Wyswietla liczbe 0 - 65535
*/
void  display_number(uint16_t liczba);

/*
Wyswietla ladnie sformatowany czas
*/
void display_time(const uint16_t * time);

#endif // LCD_H_
#ifndef LCD_H_
#define LCD_H_

#include "hd44780.h"

//********************** M A K R A *******************************

// LCD
					



//********************** F U N K C J E ***********************

void LCD_clear(void);

void LCD_carriage_return(void);

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
Wyswietla ladnie sformatowany czas
*/
void LCD_display_time(const uint16_t * time);






#endif // LCD_H_
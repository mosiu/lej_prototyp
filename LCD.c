#include "LCD.h"
#include "fun.h"
#include <stdbool.h>

#define SEND_DATA(iks)	hd44780_outdata((iks)); \
						hd44780_wait_ready(0); \
						Address++;

#define SET_ADDR(iks)	hd44780_outcmd(HD44780_DDADDR((iks))); \
						hd44780_wait_ready(0); \
						Address = (iks);

/*
Definiuje znak w CGRAM na pozycji pos
*/
void def_char(uint8_t pos, const uint8_t * char_tab);



typedef struct {
	uint8_t a[8];
	uint8_t b[8];
	uint8_t c[8];
	uint8_t d[8];
} large_digit_t;

typedef union {
	large_digit_t output;
	uint8_t input[32];
} digit_t;

static const uint8_t large_digit_tab[10][32] = {
	{
		0b00111,
		0b01111,
		0b11100,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,	// 0a
		0b11100,
		0b11110,
		0b00111,
		0b00011,
		0b00111,
		0b01011,
		0b10011,
		0b00011,	// 0b
		0b11000,
		0b11001,
		0b11010,
		0b11100,
		0b11000,
		0b11100,
		0b01110,
		0b00111,	// 0c
		0b00011,
		0b00011,
		0b00011,
		0b00011,
		0b00011,
		0b00111,
		0b11110,
		0b11100	//0d
	},
	{
		0b00011,
		0b00011,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,	// 1a
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,	// 1b
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00111,
		0b00111,	// 1c
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11111,
		0b11111	// 1d
	},
	{
		0b00011,
		0b00111,
		0b01110,
		0b01100,
		0b00000,
		0b00000,
		0b00000,
		0b00000,	// 2a
		0b11000,
		0b11100,
		0b01110,
		0b00110,
		0b00110,
		0b01110,
		0b11100,
		0b11000,	//2b
		0b00001,
		0b00011,
		0b00011,
		0b00111,
		0b00110,
		0b01110,
		0b01111,
		0b01111,	// 2c
		0b10000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b11110,
		0b11110	// 2d
	},
	{
		0b00011,
		0b00111,
		0b01110,
		0b01100,
		0b00000,
		0b00000,
		0b00000,
		0b00011,	// 3a
		0b11000,
		0b11100,
		0b01110,
		0b00110,
		0b00110,
		0b01110,
		0b11100,
		0b11000,	// 3b
		0b00011,
		0b00000,
		0b00000,
		0b00000,
		0b01100,
		0b01110,
		0b00111,
		0b00011,	// 3c
		0b11000,
		0b11100,
		0b01110,
		0b00110,
		0b00110,
		0b01110,
		0b11100,
		0b11000	// 3d
	},
	{
		0b00000,
		0b00000,
		0b00001,
		0b00001,
		0b00011,
		0b00011,
		0b00111,
		0b01110,	// 4a
		0b11000,
		0b11000,
		0b11000,
		0b10000,
		0b00000,
		0b00000,
		0b00110,
		0b00110,	// 4b
		0b01100,
		0b11111,
		0b11111,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,	// 4c
		0b0110,
		0b11111,
		0b11111,
		0b0110,
		0b0110,
		0b0110,
		0b0110,
		0b0110		// 4d
	},
	{
		0b11111,
		0b11111,
		0b11000,
		0b11000,
		0b11000,
		0b11000,
		0b11011,
		0b11111,	// 5a
		0b11110,
		0b11110,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b11000,
		0b11110,	// 5b
		0b11100,
		0b00000,
		0b00000,
		0b00000,
		0b11000,
		0b11100,
		0b01111,
		0b00111,	// 5c
		0b00111,
		0b00011,
		0b00011,
		0b00011,
		0b00011,
		0b00111,
		0b11110,
		0b11100	// 5d
	},
	{
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00001,
		0b00011,
		0b00111,
		0b01110,	// 6a
		0b00110,
		0b01110,
		0b11100,
		0b11000,
		0b10000,
		0b00000,
		0b00000,
		0b11000,	// 6b
		0b01111,
		0b11100,
		0b11000,
		0b11000,
		0b11000,
		0b11100,
		0b01111,
		0b00111,	// 6c
		0b11110,
		0b00111,
		0b00011,
		0b00011,
		0b00011,
		0b00111,
		0b11110,
		0b11100	// 6d
	},
	{
		0b01111,
		0b01111,
		0b01110,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,	// 7a
		0b11111,
		0b11111,
		0b00111,
		0b01110,
		0b01110,
		0b01100,
		0b11100,
		0b11000,	// 7b
		0b00001,
		0b00001,
		0b00011,
		0b00011,
		0b00111,
		0b00111,
		0b00111,
		0b00111,	// 7c
		0b11000,
		0b10000,
		0b10000,
		0b00000,
		0b00000,
		0b00000,
		0b00000,
		0b00000	// 7d
	},
	{
		0b00011,
		0b00111,
		0b01110,
		0b01100,
		0b01100,
		0b01110,
		0b00111,
		0b00011,	// 8a
		0b11000,
		0b11100,
		0b01110,
		0b00110,
		0b00110,
		0b01110,
		0b11100,
		0b11000,	// 8b
		0b01111,
		0b11100,
		0b11000,
		0b11000,
		0b11000,
		0b11100,
		0b01111,
		0b00011,	// 8c
		0b11110,
		0b00111,
		0b00011,
		0b00011,
		0b00011,
		0b00111,
		0b11110,
		0b11000	// 8d
	},
	{
		0b00111,
		0b01111,
		0b11100,
		0b11000,
		0b11000,
		0b11000,
		0b11100,
		0b01111,	// 9a
		0b11100,
		0b11110,
		0b00111,
		0b00011,
		0b00011,
		0b00011,
		0b00111,
		0b11110,	// 9b
		0b00011,
		0b00000,
		0b00000,
		0b00001,
		0b00011,
		0b00111,
		0b01110,
		0b01100,	// 9c
		0b01110,
		0b11100,
		0b11000,
		0b10000,
		0b00000,
		0b00000,
		0b00000,
		0b00000		// 9d
	}
};

static digit_t LargeDigit[10];


static volatile uint8_t Address = 0;	//zawsze adres pierwszego wolnego znaku LCD




void LCD_putsub(char* sub)
{
	
	if (!sub)
	{
		if (Address >= 0x40)
		{ Address = 0x40; }
		else
		{ Address = 0; }
		SET_ADDR(Address)
		
		LCD_putsub("                ");
		
		if (Address >= 0x40)
		{	Address = 0x40;	}
		else
		{	Address = 0;	}
		SET_ADDR(Address)
	}
	else
	{
		for(uint8_t i=0; (sub[i] != '\0') && (Address <= 0x80) ; i++)
		{
			if((sub[i])=='\n')
			{
				Address = 0x40;
				SET_ADDR(Address)
				continue;
			}
			SEND_DATA(sub[i])
		}
	}
	return;
}

void LCD_newline(void)
{
	if (Address >= 0x40)
	{ Address = 0x00; }
	else
	{ Address = 0x40; }
	SET_ADDR(Address)
}

void LCD_clear(void)
{
	hd44780_outcmd(HD44780_CLR);
	hd44780_wait_ready(1);
	SET_ADDR(0x00)
}

void  LCD_display_number(uint16_t number)
{
	if (!number)
	{
		SEND_DATA(48)
		return;
	}
	uint8_t digit;
	// poczatek jest bool z zastosowania
	uint8_t poczatek = false;
	for ( uint16_t divisor = 10000 ; divisor != 0 ; divisor /= 10  )
	{
		// UWAGA: 16-bitowy wynik do 8 bit!!
		digit = number / divisor ;
		// wysylam cyfry od kiedy natrafie na pierwsze niezero
		if (digit || poczatek)
		{
			SEND_DATA(48 + digit)
			poczatek = true;
		}
		number %= divisor ;
	}
}


void LCD_display_time(const uint16_t * time)
{
	// we don't measure more than 99 sec.
	uint16_t time_copy = (*time) % 10000;
	uint8_t first_large_idx;
	uint8_t second_large_idx;
	
	// display and trim first digit
	SET_ADDR(0x01)
	SEND_DATA(48 + (time_copy/1000))	
	
	// determine 1st and 2nd large digit (sec)
	time_copy %= 1000;
	first_large_idx = time_copy/100;
	
	time_copy %=100;
	second_large_idx = time_copy/10;
	
	def_char(0, LargeDigit[first_large_idx].output.a);
	def_char(1, LargeDigit[first_large_idx].output.b);
	def_char(2, LargeDigit[second_large_idx].output.a);
	def_char(3, LargeDigit[second_large_idx].output.b);
	def_char(4, LargeDigit[first_large_idx].output.c);
	def_char(5, LargeDigit[first_large_idx].output.d);
	def_char(6, LargeDigit[second_large_idx].output.c);
	def_char(7, LargeDigit[second_large_idx].output.d);
	
	// display upper line
	SET_ADDR(0x02)
	SEND_DATA(0)
	SEND_DATA(1)
	SET_ADDR(0x05)
	SEND_DATA(2)
	SEND_DATA(3)
	
	// display lower line (excluding already displayed 10sec digit)
	SET_ADDR(0x42)
	SEND_DATA(4)
	SEND_DATA(5)
	SEND_DATA('.')
	SEND_DATA(6)
	SEND_DATA(7)
	SEND_DATA(48 + (time_copy % 10))
}

// CAUTION! Everytime you call this, the DDRAM address needs to be set again
void def_char(uint8_t pos, const uint8_t * char_tab)
{	
	hd44780_outcmd(HD44780_CGADDR(pos*8));
	hd44780_wait_ready(0);
	
	for(uint8_t i=0; i<8; i++)
	{
		hd44780_outdata(char_tab[i]);
		hd44780_wait_ready(0);
	}
}

void LCD_init(void)
{
	hd44780_init();
	
	hd44780_outcmd(HD44780_CLR);
	hd44780_wait_ready(1);
	hd44780_outcmd(HD44780_FNSET(0,1,0));
	hd44780_wait_ready(0);
	hd44780_outcmd(HD44780_DISPCTL(1,1,0));
	hd44780_wait_ready(0);
	hd44780_outcmd(HD44780_ENTMODE(1,0));
	hd44780_wait_ready(0);
	
	// load large digits from FLASH to... I don't know what yet.
	for(uint8_t i=0; i<10; i++)
	{
		for(uint8_t j=0; j<32; j++)
		{
			LargeDigit[i].input[j] = large_digit_tab[i][j];
		}
	}
	
	LCD_clear();
}

void LCD_carriage_return(void)
{
		if (Address >= 0x40)
		{ Address = 0x40; }
		else
		{ Address = 0x00; }
		SET_ADDR(Address)
}


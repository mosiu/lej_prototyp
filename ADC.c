/*
 * ADC.c
 *
 * Created: 11/29/2015 8:20:53 PM
 *  Author: mosiu
 */ 

#include "ADC.h"

#define ADC_MUX_MASK 0
static volatile uint8_t ADC_result = 0u;


void ADC_init(void)
{
	
	// enable ADC
	ADCSRA |= (1 << ADEN);
	// select input
	ADMUX &= ~0xF;
	ADMUX |= ADC_MUX_MASK;
	// select AVCC as reference
	ADMUX &= ~(1 << REFS1)|(1 << REFS0);
	ADMUX |= (1 << REFS0);
	// select left-adjustment to read only ADCH (8-bit precision)
	ADMUX |= (1 << ADLAR);
	// select free running mode
	ADCSRA |= (1 << ADFR);
	// slow down ADC clock by 16
	ADCSRA |= (1 << ADPS2);	
	// start first conversion
	ADCSRA |= (1 << ADSC);
	// enable ADC interrupt
	ADCSRA |= (1 << ADIE);	
}

ISR(ADC_vect, ISR_BLOCK)
{
	ADC_result = ADCH;
	return;
}	

uint8_t ADC_get_result(void)
{
	return ADC_result;
}


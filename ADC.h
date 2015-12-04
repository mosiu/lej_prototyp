/*
 * ADC.h
 *
 * Created: 11/29/2015 8:21:07 PM
 *  Author: mosiu
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void ADC_init(void);

uint8_t ADC_get_result(void);



#endif /* ADC_H_ */
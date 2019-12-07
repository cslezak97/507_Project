/*
 * TMP_36_Xmega.h
 *
 * Created: 12/4/2019 11:46:12 AM
 *  Author: cslez
 */ 


#ifndef TMP_36_XMEGA_H_
#define TMP_36_XMEGA_H_

// #define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <stdlib.h>

uint16_t VREF ; // Internal reference voltage of 1 V or 1000mV
uint16_t DELTAV ; // Voltage offset in mV
uint16_t BITCOUNT;
uint16_t FARCONS;

uint32_t Reading;
uint32_t ReadingmV ;
uint16_t Voltage ;
uint16_t Temp_dC ;
uint16_t Temp_dF ;


int8_t ReadSignatureByte(int16_t Address);
int16_t ReadADC(int8_t Channel, int8_t ADCMode);
uint16_t readTemp();


#endif /* TMP_36_XMEGA_H_ */
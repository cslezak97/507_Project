#define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/delay.h>
#include <stdlib.h>



int8_t ReadSignatureByte(int16_t Address)
{
	NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
	uint8_t Result;
	__asm__ ("lpm %0, Z\n" : "=r" (Result) : "z" (Address));
	NVM_CMD = NVM_CMD_NO_OPERATION_gc;
	return Result;
}

int16_t ReadADC(int8_t Channel, int8_t ADCMode) // Mode = 1 for single ended, 0 for internal
{
	if ((ADCA.CTRLA & ADC_ENABLE_bm) == 0)
	{
		ADCA.CTRLA = ADC_ENABLE_bm ; // Enable the ADC
		ADCA.CTRLB = (0<<4); // unSigned Mode
		ADCA.REFCTRL = 0; // Internal 1v ref
		ADCA.EVCTRL = 0 ; // no events
		ADCA.PRESCALER = ADC_PRESCALER_DIV128_gc ;
		ADCA.CALL = ReadSignatureByte(0x0C) ; //ADC Calibration Byte 0
		ADCA.CALH = ReadSignatureByte(0x0D) ; //ADC Calibration Byte 1
		//ADCA.SAMPCTRL = This register does not exist
		//_delay_us(400); // Wait at least 25 clocks
	}
	ADCA.CH3.CTRL = ADC_CH_GAIN_1X_gc | ADCMode ; // Gain = 1, Single Ended
	ADCA.CH3.MUXCTRL = (Channel<<3);
	ADCA.CH3.INTCTRL = 0 ; // No interrupt
	//ADCA.CH0.SCAN Another bogus register
	for(int8_t Waste = 0; Waste<2; Waste++)
	{
		ADCA.CH3.CTRL |= ADC_CH_START_bm; // Start conversion
		while (ADCA.INTFLAGS==0) ; // Wait for complete
		ADCA.INTFLAGS = ADCA.INTFLAGS ;
	}
	return ADCA.CH3RES ;
}

uint16_t readTemp (void)
{
	uint16_t VREF =  1000; // Internal reference voltage of 1 V or 1000mV
	uint16_t DELTAV = 27; // Voltage offset in mV
	uint16_t BITCOUNT = 4096;
	uint16_t FARCONS = 320; // Y intercept for Celsius-->Farenheit conversion

	uint32_t Reading = ReadADC(3, 1); //Reading from the ADC, BITS
	uint32_t ReadingmV = Reading * VREF;
	uint16_t Voltage = (ReadingmV/BITCOUNT) - DELTAV; //Voltage at pin in mV
	uint16_t Temp_dC = (Voltage - 500); //Temperature in deg deci Celsius
	uint16_t Temp_dF = (Temp_dC * 1.8 + FARCONS); //Temperature in deg deci Farenheit

	return Temp_dF;
}

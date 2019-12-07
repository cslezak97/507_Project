/*
 * Motors.cpp
 *
 * Created: 12/2/2019 8:43:25 PM
 *  Author: cslez
 */ 
#include <avr/io.h>                         // Port I/O for SFR's
#include <avr/wdt.h>                        // Watchdog timer header
#include <avr/interrupt.h>
#include <string.h>                         // Functions for C string handling
#include <asf.h>

#include "Motors.h"                      // Header for this file



void Lid_Init(void)
{
	//for motor 1 (lid motor)
	PORTC_DIRSET=PIN2_bm; //set PinC2 as output (pwmb)
	PORTD_DIRSET=PIN7_bm; //set PinD7 as output (bin2)
	PORTC_DIRSET=PIN7_bm; //set PinC7 as output (bin1)
	PORTC_DIRSET=PIN6_bm; //set PinC6 as output (stby1)
}

void Lid_Rotate(void)
{
	struct pwm_config pwm_cfg_1;
	pwm_init(&pwm_cfg_1, PWM_TCC0, PWM_CH_C, 500);
	
	PORTC_OUTSET=PIN2_bm; //set PinC2 as high (pwmb)
	PORTD_OUTSET=PIN7_bm; //set PinD7 as high (bin2)
	PORTC_OUTCLR=PIN7_bm; //set PinC7 as low (bin1)
	PORTC_OUTSET=PIN6_bm; //set PinC6 as high (stby1)
	
	pwm_start(&pwm_cfg_1, 50); //start pwm with 50% duty cycle
}

void Lid_Stop(void)
{
	struct pwm_config pwm_cfg_1;
	pwm_init(&pwm_cfg_1, PWM_TCC0, PWM_CH_C, 500);
	
	PORTC_OUTSET=PIN2_bm; //set PinC2 as high (pwmb)
	PORTD_OUTCLR=PIN7_bm; //set PinD7 as high (bin2)
	PORTC_OUTCLR=PIN7_bm; //set PinC7 as low (bin1)
	PORTC_OUTSET=PIN6_bm; //set PinC6 as high (stby1)
	
	pwm_disable(&pwm_cfg_1);
}

void Roller_Init(void)
{
	//for motor 2 (roller motor)
	PORTD_DIRSET=PIN0_bm; //set PinD0 as output (pwma)
	PORTE_DIRSET=PIN2_bm; //set PinE2 as output (ain2)
	PORTE_DIRSET=PIN1_bm; //set PinE1 as output (ain1)
	PORTE_DIRSET=PIN0_bm; //set PinE0 as output (stby)
}

void Roller_Rotate(void)
{
	struct pwm_config pwm_cfg_2;
	//sysclk_init();
	pwm_init(&pwm_cfg_2, PWM_TCD0, PWM_CH_A, 500);
	
	PORTD_OUTSET=PIN0_bm; //set PinD0 as high (pwm)
	PORTE_OUTSET=PIN2_bm; //set PinE2 as high (ain2)
	PORTE_OUTCLR=PIN1_bm; //set PinE1 as low (ain1)
	PORTE_OUTSET=PIN0_bm; //set PinE0 as high (stby)
	
	pwm_start(&pwm_cfg_2, 95); //start pwm with 90% duty cycle
}

void Roller_Stop(void)
{
	struct pwm_config pwm_cfg_2;
	//sysclk_init();
	pwm_init(&pwm_cfg_2, PWM_TCD0, PWM_CH_A, 500);
	
	PORTD_OUTSET=PIN0_bm; //set PinD0 as high (pwm)
	PORTE_OUTCLR=PIN2_bm; //set PinE2 as high (ain2)
	PORTE_OUTCLR=PIN1_bm; //set PinE1 as low (ain1)
	PORTE_OUTSET=PIN0_bm; //set PinE0 as high (stby)
	
	//pwm_start(&pwm_cfg_2, 0); //start pwm with 0% duty cycle
	pwm_disable(&pwm_cfg_2);
}
/*
 * Lid_Motor.h
 *
 * Created: 12/2/2019 8:43:04 PM
 *  Author: cslez
 */ 


#ifndef Motors_H_
#define Motors_H_

#include <stdlib.h>                         // Prototype declarations for I/O functions
#include <asf.h>

//initialize the Lid Pins 
void Lid_Init(void);

//set Lid Pins to high or low for motor to rotate
void Lid_Rotate(void);

//Stop motor by disabling pwm and setting pins to low or high 
void Lid_Stop(void);

//initialize the Roller Pins
void Roller_Init(void);

//set Roller Pins to high or low for motor to rotate
void Roller_Rotate(void);

//Stop motor by disabling pwm and setting pins to low or high
void Roller_Stop(void);


#endif /* Motors_H_ */
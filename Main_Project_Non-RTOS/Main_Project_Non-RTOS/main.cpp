/*
 * Main_Project_Non-RTOS.cpp
 *
 * Created: 12/4/2019 9:42:27 AM
 * Author : cslez
 */ 

#define F_CPU 32000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <asf.h>
#include "Motors.h"
#include "LiquidCrystal_I2C.h"


#include <stdio.h>

int main(void)
{
	char MESSAGE[15] = "Push Button To";
	char MESSAGE_2[6] = "Start";

	uint16_t length = sizeof(MESSAGE)-1;
	uint16_t length_2 = sizeof(MESSAGE_2)-1;

	
	LiquidCrystal_I2C lcd(0x3F,16,2); //address of device, no. rows, no. columns. All start at 0.
	lcd.begin();
	lcd.clear();
	lcd.setBacklight(1);
	lcd.setCursor(0,0); //row,column
	lcd.printMessage(&MESSAGE[0],length);
	lcd.setCursor(5,1);
	lcd.printMessage(&MESSAGE_2[0],length_2);
	
	Lid_Init();
	Roller_Init();
	//for L.S#1
	PORTA.DIRCLR=PIN7_bm; //set PinA7 as input- Limit Switch 1
	PORTA.OUTSET=PIN7_bm; //set the initial value of PinA7 as high- Limit Switch
	
	//for L.S#2
	PORTA.DIRCLR=PIN6_bm; //set PinA7 as input- Limit Switch 1
	PORTA.OUTSET=PIN6_bm; //set the initial value of PinA7 as high- Limit Switch
	
	//for Pushbutton
	PORTB.DIRCLR=PIN3_bm; //set PinB3 as input- Pushbutton
	PORTB.OUTSET=PIN3_bm; //set the initial value of PinB3 as high- Pushbutton
	
	//for Breakbeam
	PORTB_IN=PIN2_bm; //set PinB2 as input. - Breakbeam
		
	//for Solenoid	
	PORTB_DIRSET=PIN0_bm; //set PinB0 as output. -Solenoid
	
	uint32_t cooling_time; //set variable cooling_time
	uint16_t  state; //set state variable
	uint16_t previous_state;
	
	cooling_time=0;
	state=0;
	previous_state=0;

    while (1) 
    {
		//init state stopping both motors
		while (state==0)
		{
			lcd.setCursor(0,0); //row,column
			lcd.printMessage(&MESSAGE[0],length);
			lcd.setCursor(5,1);
			lcd.printMessage(&MESSAGE_2[0],length_2);
			Lid_Stop();
			Roller_Stop();
			state=1;
			break;
		}
		
		//init state waiting for pushbutton to be pressed
		while (state==1)
		{
			if (!(PORTB.IN & PIN3_bm))
			{
				PORTB_OUTSET=PIN0_bm; //opens solenoid to allow can to fall through
				_delay_ms(50); //allows the can to start falling through before checking breakbeam
				state=2;
			}
			else
			{
				state=1;
			} //pass
			break;
		}
		
		//state to make sure can has fallen through and to close solenoid.
		while (state==2)
		{
			if ((PORTB.IN & PIN2_bm)) //checks for if can has finished falling through
			{
				state=3;
				PORTB_OUTCLR=PIN0_bm; //closes the solenoid
			}
			else
			{
				lcd.printTemp();
				state=2;				
			}
		}
		
		
		//state to run the lid motor
		while (state==3)
		{
			lcd.clear();
			Lid_Rotate();
			state=4;
			break;
		}
		
		//state that checks for limit switch to be pressed
		while (state==4)
		{
			if (PORTA.IN & PIN7_bm) //L.S 1->Roller Limit Switch
			{
				Lid_Stop();
				state=5;
				_delay_ms(100);
			}
			else
			{
				lcd.printTemp();
				state=4;
			} //pass
			break;
		}
		
		
		//state to run the roller motor
		while (state==5)
		{
			lcd.clear();
			Roller_Rotate();
			cooling_time=0;
			state=6;
			break;
		}
		
		//state that runs for 30s before stopping roller motors and transitioning to opening lid
		while (state==6)
		{
			if (cooling_time>330)
			{
				Roller_Stop();
				state=7;
			}
			else
			{
				lcd.printTemp();
				cooling_time++;
				state=6;
			}
			break;
		}
		
		
		//state to open the lid up
		while (state==7)
		{
			lcd.clear();
			Lid_Rotate();
			state=8;
		}
		
		
		//state to check for when limit switch is pressed (lid fully open)
		while (state==8)
		{
			if (PORTA.IN & PIN6_bm) //L.S 2->Lid Limit Switch
			{
				PORTA.OUTCLR=PIN6_bm; //set the initial value of PinA6 as 0 (Pushbutton)
				Lid_Stop();
				state=0;
				lcd.clear();
			}
			else
			{
				lcd.printTemp();
				state=8;
			} //pass
			break;
		}
    }
}


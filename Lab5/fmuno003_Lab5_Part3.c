/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #3 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "bit.h"

unsigned int i = 0;
unsigned int j = 0;
unsigned char pattern = 0x01;
unsigned char pattern1 = 0x01;
int power = 0;
int power1 = 0;
enum Pattern_States{init1, on} state;
enum Pattern1_States{init11, on11} State;
enum Pattern_States2{init2, on2, off2} states;
enum Pattern1_States2{init22, on22, off22} states2;
enum Pattern_States3{init3, on3, off3} state1;
enum Pattern1_States3{init33, on33, off33} state11;
enum Button_Press{init, up, down, turnOff, wait11, turnOn} btnState;
enum Button1_Press{init01, up1, down1, turnOff1, wait1, turnOn1} btnState1;
void transmit_data(unsigned char data)
{
	unsigned char i;
	for (i = 0; i < 8 ; ++i)
	{
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
	PORTC |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}
void transmit_data1(unsigned char data)
{
	unsigned char i;
	for (i = 0; i < 8 ; ++i)
	{
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTD = 0x08;
		// set SER = next bit of data to be sent.
		PORTD |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTD |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
	PORTD |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTD = 0x00;
}
void Tick_Button()
{
	
	switch(btnState)
	{
		case init:
			if(power && GetBit(PINA, 0))
				btnState = up;
			else if(power && GetBit(PINA, 1))
				btnState = down;
			else if(power && GetBit(PINA, 1) && GetBit(PINA, 0))
				btnState = turnOff;
			else if(!power && GetBit(PINA, 1) && GetBit(PINA, 0))
				btnState = turnOn;			
			else
				btnState = init;
			break;
		case up:
			if(power && GetBit(PINA, 0))
				btnState = wait11;
			break;
		case down:
			if(power && GetBit(PINA, 1))
				btnState = wait11;
			break;
		case turnOff:
			if(power && GetBit(PINA, 0) && GetBit(PINA, 1))
				btnState = init;
			break;
		case wait11:
			if(GetBit(PINA, 0) && GetBit(PINA, 1))
				btnState = init;
			break;
		case turnOn:
			if(!power && GetBit(PINA, 1) && GetBit(PINA, 0))
				btnState = init;
			break;
		default:
			break;
	}
	switch(btnState)
	{
		case init:
			break;
		case up:
			if(i < 4)
				++i;
			break;
		case down:
			if(i > 0)
				--i;
			break;
		case turnOff:
			power = 0;
			break;
		case turnOn:
			power = 1;
			break;
		case wait11:
			break;
		default:
			break;
	}
}
void Tick1_Button()
{
	switch(btnState1)
	{
		case init01:
			if(power1 && GetBit(PINA, 2))
				btnState1 = up1;
			else if(power1 && GetBit(PINA, 3))
				btnState1 = down1;
			else if(power1 && GetBit(PINA, 3) && GetBit(PINA, 2))
				btnState1 = turnOff1;
			else if(!power1 && GetBit(PINA, 3) && GetBit(PINA, 2))
				btnState1 = turnOn1;			
			else
				btnState1 = init01;
			break;
		case up1:
			if(power1 && GetBit(PINA, 2))
				btnState1 = wait1;
			break;
		case down1:
			if(power1 && GetBit(PINA, 3))
				btnState1 = wait1;
			break;
		case turnOff1:
			if(power1 && GetBit(PINA, 2) && GetBit(PINA, 3))
				btnState1 = init;
			break;
		case wait1:
			if(GetBit(PINA, 2) && GetBit(PINA, 3))
				btnState1 = init;
			break;
		case turnOn1:
			if(!power1 && GetBit(PINA, 3) && GetBit(PINA, 2))
				btnState1 = init;
			break;
		default:
			break;
	}
	switch(btnState1)
	{
		case init01:
			break;
		case up1:
			if(j < 4)
				++j;
			break;
		case down1:
			if(j > 0)
				--j;
			break;
		case turnOff1:
			power1 = 0;
			break;
		case turnOn:
			power1 = 1;
			break;
		case wait1:
			break;
		default:
			break;
	}
}
void Tick_Pattern1()
{
	switch(state)
	{
    	case init1:
			if(i == 0)
     			state = on;
    		break;
    	case on:
			if(i != 0)
				state = init1;
			else
				state = on;
			break;
		default:
			break;
	}
	switch(state)
	{
		case init1:
			break;
		case on:
			for(int j = 0; j < 256; ++j)
			{
				transmit_data(j);
			}
			for(int j = 255; j > 0; --j)
			{
				transmit_data(j);
			}
			break;
		default:
			break;
	}
}
void Tick1_Pattern1()
{
	switch(State)
	{
    	case init11:
			if(j == 0)
     			State = on11;
    		break;
    	case on11:
			if(j != 0)
				State = init1;
			else
				state = on11;
			break;
		default:
			break;
	}
	switch(State)
	{
		case init11:
			break;
		case on11:
			for(int k = 0; k < 256; ++k)
			{
				transmit_data1(k);
			}
			for(int k = 255; j > k; --k)
			{
				transmit_data1(k);
			}
			break;
		default:
			break;
	}
}
void Tick_Pattern2()
{
	switch(states)
	{
		case init2:
			if(i == 1)
				states = on2;
			break;
		case on2:
			if(i != 1)
				states = init2;
			else
				states = off2;
			break;
		case off2:
			if(i != 1)
				states = init2;
			else
				states = on2;
		default:
			break;
	}
	switch(states)
	{
		case init2:
			break;
		case on2:
			transmit_data(0xAA);
			break;
		case off2:
			transmit_data(0x55);
			break;
		default:
			break;
	}
}
void Tick1_Pattern2()
{
	switch(states2)
	{
		case init22:
			if(j == 1)
				states2 = on22;
			break;
		case on2:
			if(j != 1)
				states2 = init22;
			else
				states2 = off22;
			break;
		case off2:
			if(j != 1)
				states2 = init22;
			else
				states2 = on22;
		default:
			break;
	}
	switch(states2)
	{
		case init22:
			break;
		case on22:
			transmit_data1(0xAA);
			break;
		case off22:
			transmit_data1(0x55);
			break;
		default:
			break;
	}
}
void Tick_Pattern3()
{
	switch(state1)
	{
		case init3:
			state1 = on3;
			break;
		case on3:
			if(i != 2)
				state1 = init3;
			else if(i == 2 && pattern != 0x80)
				state1 = on3;
			else if(i == 2 && pattern == 0x80)
				state1 = off3;
			break;
		case off3:
			if(i != 2)
				state1 = init3;
			else if(i == 2 && pattern != 0x01)
				state1 = on3;
			else if(i == 2 && pattern == 0x01)
				state1 = off3;
			break;
		default:
			break;
	}
	switch(state1)
	{
		case init3:
			break;
		case on3:
			pattern = pattern << 1;
			transmit_data(pattern);
			break;
		case off3:
			pattern = pattern >> 1;
			transmit_data(pattern);
			break;
		default:
			break;
	}
}
void Tick1_Pattern3()
{
	switch(state11)
	{
		case init33:
			state11 = on33;
			break;
		case on33:
			if(j != 2)
				state11 = init33;
			else if(j == 2 && pattern1 != 0x80)
				state11 = on33;
			else if(j == 2 && pattern1 == 0x80)
				state11 = off33;
			break;
		case off33:
			if(j != 2)
				state11 = init33;
			else if(j == 2 && pattern1 != 0x01)
				state11 = on33;
			else if(j == 2 && pattern1 == 0x01)
				state11 = off33;
			break;
		default:
			break;
	}
	switch(state11)
	{
		case init33:
			break;
		case on33:
			pattern1 = pattern1 << 1;
			transmit_data1(pattern1);
			break;
		case off33:
			pattern1 = pattern1 >> 1;
			transmit_data1(pattern1);
			break;
		default:
			break;
	}
}
void Shift_Task()
{
	btnState = init;
	for(;;)
	{
		Tick_Button();
		vTaskDelay(250);
	}
}
void Shift2_Task()
{
	
	btnState1 = init;
	for(;;)
	{
		Tick1_Button();
		vTaskDelay(250);
	}
}
void Pattern1_Task()
{
	state = init1;
	for(;;)
	{
		Tick_Pattern1();
		vTaskDelay(250);
	}
}
void Pattern1_Task1()
{
	State = init11;
	for(;;)
	{
		Tick1_Pattern1();
		vTaskDelay(250);
	}
}
void Pattern2_Task()
{
	states = init2;
	for(;;)
	{
		Tick_Pattern2();
		vTaskDelay(250);
	}
}
void Pattern2_Task1()
{
	states = init22;
	for(;;)
	{
		Tick1_Pattern2();
		vTaskDelay(250);
	}
}
void Pattern3_Task()
{
	state1 = init3;
	for(;;)
	{
		Tick_Pattern3();
		vTaskDelay(250);
	}
}
void Pattern3_Task1()
{
	state1 = init33;
	for(;;)
	{
		Tick1_Pattern3();
		vTaskDelay(250);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Shift_Task, (signed portCHAR *)"Shift_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void Start1ShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Shift2_Task, (signed portCHAR *)"Shift2_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void StartPatternPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern1_Task, (signed portCHAR *)"Pattern1_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void Start1PatternPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern1_Task1, (signed portCHAR *)"Pattern1_Task1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void StartPattern1Pulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern2_Task, (signed portCHAR *)"Pattern2_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void Start1Pattern1Pulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern2_Task1, (signed portCHAR *)"Pattern2_Task1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void StartPattern2Pulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern3_Task, (signed portCHAR *)"Pattern3_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void Start1Pattern2Pulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern3_Task1, (signed portCHAR *)"Pattern3_Task1", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0xFF;
	StartShiftPulse(1);
	StartPatternPulse(1);
	StartPattern1Pulse(1);
	StartPattern2Pulse(1);
	Start1ShiftPulse(1);
	Start1PatternPulse(1);
	Start1Pattern1Pulse(1);
	Start1Pattern2Pulse(1);
	vTaskStartScheduler();
	return 0;
}

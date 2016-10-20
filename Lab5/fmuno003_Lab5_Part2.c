/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #2 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/eeprom.h>
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "bit.h"

unsigned int i = 0;
unsigned char pattern = 0x01;
int power = 0;
enum Pattern_States{init1, on} state;
enum Pattern_States2{init2, on2, off2} states;
enum Pattern_States3{init3, on3, off3} state1;
enum Button_Press{init, up, down, turnOff, turnOn, wait1} btnState;
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
				btnState = wait1;
			break;
		case down:
			if(power && GetBit(PINA, 1))
				btnState = wait1;
			break;
		case turnOff:
			if(GetBit(PINA, 0) || GetBit(PINA, 1))
				btnState = wait1;
			break;
		case wait1:
			if(!GetBit(PINA, 0) && !GetBit(PINA, 1))
				btnState = init;
			break;
		case turnOn:
			if(GetBit(PINA, 1) || GetBit(PINA, 0))
				btnState = wait1;
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
void Shift_Task()
{
	btnState = init;
	for(;;)
	{
		Tick_Button();
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
void Pattern2_Task()
{
	states = init2;
	for(;;)
	{
		Tick_Pattern2();
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
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Shift_Task, (signed portCHAR *)"Shift_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void StartPatternPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern1_Task, (signed portCHAR *)"Pattern1_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void StartPattern1Pulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern2_Task, (signed portCHAR *)"Pattern2_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
void StartPattern2Pulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern3_Task, (signed portCHAR *)"Pattern3_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main()
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	StartShiftPulse(1);
	StartPatternPulse(1);
	StartPattern1Pulse(1);
	StartPattern2Pulse(1);
	vTaskStartScheduler();
	return 0;
}

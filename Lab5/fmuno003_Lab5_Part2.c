/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #2 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
 
 
#include <stdint.h>
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
#include "usart_ATmega1284.h"

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
int power = 0;
enum Pattern_States{init, on} state;
enum Pattern_States2{init2, on} states;
enum Pattern_States3{iinit3, on} state1;
enum Button_Press{init, up, down, off} State;

void Tick_Button()
{
	switch(State)
	{
		case init:
			if(power && (~PINA & 0x01 == 0x01))
			{
				State = up;
			}
			else if(power && (~PINA & 0x02 == 0x02))
			{
				State = down;
			}
			else if(power && (~PINA & 0x03 == 0x03))
			{
				State = off;
			}
			else
			{
				State = init;
			}
			break;
		case up:
			if(power && (~PINA & 0x01 == 0x01))
			{
				State = wait;
			}
			break;
		case down:
			if(power && (~PINA & 0x02 == 0x02))
			{
				State = wait;
			}
			break;
		case off:
			if(~PINA & 0x03 == 0x03)
			{
				State = powerOff;
			}
			break;
		case wait:
			if(~PINA & 0x03 == 0x00)
			{
				State = init;
			}
			break;
		case powerOff:
			if(!power && (~PINA & 0x03 == 0x03))
			{
				State = init;
			}
			break;
		default:
			break;
	}
	
void Tick_Pattern1()
{
	switch(state)
	{
    	case init:
     		state = on;
    		break;
    	case on:
			state = on;
			break;
		default:
			break;
	}
	switch(state)
	{
		case init:
			break;
		case on:
			for(i = 0; i < 256; ++i)
			{
				transmit_data(array[i]);
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
			state = on2;
			break;
		case on2:
			state = on2;
			break;
		default:
			break;
	}
	switch(states)
	{
		case init2:
			break;
		case on2:
			for(j = 0; j < 30; ++j)
			{
				transmit_data(arrays[j]);
			}
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
			state1 = on3;
			break;
		default:
			break;
	}
	switch(state1)
	{
		case init3:
			break;
		case on3:
			for(k = 0; k < 15; ++k)
			{
				transmit_data(patternArray[k]);
			}
			break;
		default:
			break;
	}
}

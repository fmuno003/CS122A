/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: Servant
 *	Debug Some code
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "bit.h"
#include "keypad.h"
#include "lcd.h"

unsigned char receivedData;
unsigned char temp;
unsigned char pattern;
unsigned char speed;
unsigned char clr_highNibble = 0x0F;
unsigned char clr_lowNibble = 0xF0;
unsigned char btn;
unsigned char data = 0x11;
unsigned char pattern3 = 0x01;
unsigned char pattern4 = 0xF0;

enum servantStates {s_wait, read} states;
enum pattern1States {wait1, light1, light2} lightState;
enum pattern2States {wait2, light3, light4} lightState2;
enum pattern3States {wait3, shift_left, shift_right} lightState3;
enum pattern4States {wait4, shift_left4, shift_right4} lightState4;

void transmit_data(unsigned char data) 
{
	unsigned char i;
	for (i = 0; i < 8 ; ++i) {
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
void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x40; PORTB = 0xBF;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= (1<<SPE) | (1<<SPIE);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}
ISR(SPI_STC_vect){ // this is enabled in with the SPCR register?s ?SPI
	// Interrupt Enable?
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;
	receivedData = SPDR;
}
void TickFct_servant()
{
	switch( state )
	{
		case s_wait:
			state = read;
			break;
		case read:
			state = s_wait;
			break;
		default:
			state = s_wait;
			break;
	}
	switch( state )
	{
		case s_wait:
			break;
		case read:
			temp = receivedData;
			speed = temp & 0x0F;
			pattern = (temp >> 4) & 0x0F;
			if(pattern != 0x00)
			{
				Pattern_Task(pattern);
			}
			break;
		default:
			break;
	}
}
void TickFct_pattern1() 
{
	switch( lightState )
	{
		case wait1:
			if(pattern == 1)
				lightState = light1;
			else
				lightState = wait1;
			break;
		case light1:
			if(pattern != 1)
				lightState = wait1;
			else if(pattern == 1)
				lightState = light2;
			break;
		case light2:
			if(pattern != 1)
				lightState = wait1;
			else if(pattern == 1)
				lightState = light1;
			break;
		default:
			break;
	}
	switch( lightState )
	{
		case wait1:
			break;
		case light1:
			PORTC = transmit_data(pattern | 0x0E);
			break;
		case light2:
			PORTC = transmit_data(0xF0);
			break;
		default:
			break;
	}
}
void TickFct_pattern2()
{
	switch( lightState2 )
	{
		case wait2:
			if(pattern == 2)
				lightState2 = light3;
			else
				lightState2 = wait2;
			break;
		case light3:
			if(pattern != 2)
				lightState2 = wait2;
			else if(pattern == 2)
				lightState2 = light4;
			break;
		case light4:
			if(pattern != 2)
				lightState2 = wait2;
			else if(pattern == 2)
				lightState2 = light3;
			break;
		default:
			break;
	}
	switch( lightState2 )
	{
		case wait2:
			break;
		case light3:
			PORTC = transmit_data(pattern | 0xA8);
			break;
		case light4:
			PORTC = transmit_data(0x55);
			break;
		default:
			break;
	}
}
void TickFct_pattern3()
{
	switch( lightState3 )
	{
		case wait3:
			if(pattern == 3)
			{
				pattern3 = 0x80;
				lightState3 = shift_right;
			}
			else
				lightState3 = wait3;
			break;
		case shift_left:
			if(pattern == 3 && pattern3 != 0x80)
				lightState3 = shift_left;
			else if(pattern ==3 && pattern3 == 0x80)
				lightState3 = shift_right;
			else if(pattern != 3)
				lightState3 = wait3;
			break;
		case shift_right:
			if(pattern == 3 && pattern3 != 0x01)
				lightState3 = shift_right;
			else if(pattern == 3 && pattern3 == 0x01)
				lightState3 = shift_left;
			else if(pattern != 3)
				lightState3 = wait3;
			break;
		default:
			break;
	}
	switch( lightState3 )
	{
		case wait3:
			pattern3 = transmit_data(0x00);
			break;
		case shift_left:
			pattern3 = pattern3 << 1;
			PORTC = transmit_data(pattern3);
			break;
		case shift_right:
			pattern3 = pattern3 >> 1;
			PORTC = transmit_data(pattern3);
			break;
		default:
			break;
	}
}
void TickFct_pattern4()
{
	switch( lightState4 )
	{
		case wait4:
			if(pattern == 4)
				lightState4 = shift_right4;
			else
				lightState4 = wait4;
			break;
		case shift_left4:
			if(pattern == 4 && pattern4 != 0xF0)
				lightState4 = shift_left4;
			else if(pattern == 4 && pattern4 == 0xF0)
				lightState4 = shift_right4;
			else if(pattern != 4)
				lightState4 = wait4;
			break;
		case shift_right4:
			if(pattern == 4 && pattern4 != 0x0F)
				lightState4 = shift_right4;
			else if(pattern == 4 && pattern4 == 0x0F)
				lightState4 = shift_left4;
			else if(pattern != 4)
				lightState4 = wait4;
			break;
		default:
			break;
	}
	switch( lightState4 )
	{
		case wait4:
			break;
		case shift_left4:
			pattern4 = pattern4 << 1;
			PORTC = transmit_data(pattern4);
			break;
		case shift_right4:
			pattern4 = pattern4 >> 1;
			PORTC = transmit_data(pattern4);
			break;
		default:
			break;
	}
}
void Servant_Task()
{
	state = s_wait;
	for(;;)
	{
		TickFct_servant();
	}
}
void Pattern_Task(unsigned char pattern)
{
	while(pattern != 0x00)
	{
		if(pattern == 0x01)
		{
			lightState = wait1;
			while(pattern == 0x01)
			{
				TickFct_pattern1();
			}
		}
		if(pattern == 0x02)
		{
			lightState2 = wait2;
			while(pattern == 0x02)
			{
				TickFct_pattern2();
			}
		}
		if(pattern == 0x03)
		{
			lightState3 = wait3;
			while(pattern == 0x03)
			{
				TickFct_pattern3();
			}
		}
		if(pattern == 0x04)
		{
			lightState4 = wait4;
			while(pattern == 0x04)
			{
				TickFct_pattern4();
			}
		}
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Servant_Task, (signed portCHAR *)"Servant_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
		// SLAVE
        SPI_ServantInit();
        DDRC = 0xFF; PORTC = 0x00;
        DDRA = 0xFF; PORTA = 0x00;
		StartSecPulse(1);
		vTaskStartScheduler();
		
	return 0;       
}

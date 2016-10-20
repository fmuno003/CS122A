/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #4 
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

unsigned short LED = 0x0010;
enum LED_States{shiftLeft, shiftRight} LEDstate;
void transmit_data(unsigned short data)
{
	for(int i = 0; i < 16; ++i)
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
void Tick_LED()
{
	switch(LEDstate)
	{
		case shiftLeft:
			if(LED != 0x8000)
				LEDstate = shiftLeft;
			else if(LED == 0x8000)
				LEDstate = shiftRight;
			break;
		case shiftRight:
			if(LED != 0x0001)
				LEDstate = shiftRight;
			else if(LED == 0x0001)
				LEDstate = shiftLeft;
			break;
		default: 
			break;
	}
	switch(LEDstate)
	{
		case shiftLeft:
			LED = LED << 1;
			transmit_data(LED);
			break;
		case shiftRight:
			LED = LED >> 1;
			transmit_data(LED);
			break;
		default:
			break;
	}
}
void Shift_Task()
{
	LEDstate = shiftLeft;
	for(;;)
	{
		Tick_LED();
		vTaskDelay(250);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Shift_Task, (signed portCHAR *)"Shift_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main()
{
	DDRC = 0xFF; PORTC = 0x00;
	
	StartShiftPulse(1);
	vTaskStartScheduler();
	return 0;
}

/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #2 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "bit.h"

enum DemoStates(init, shift_left, shift_right, wait_left, wait_right) LEDstate;

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
void Demo_Tick()
{
	static unsigned char pattern = 0x80;
	static unsigned char row = 0x00;
	switch(LEDstate)
	{
		case init:
			if(!GetBit(PINA, 0))
				LEDstate = wait_left;
			else if(!GetBit(PINA, 1))
				LEDstate = wait_right;
			break;
		case wait_left:
			if(!GetBit(PINA, 0))
				LEDstate = wait_left;
			else
				LEDstate = shift_right;
			break;
		case wait_right:
			if(!GetBit(PINA,1))
				LEDstate = wait_right;
			else
				LEDstate = shift_right;
			break;
		case shift_left:
			LEDstate = init;
			break;
		case shift_right:
			LEDstate = init;
			break;
		default:
			LEDstate = init;
			break;
		
	}
	switch(LEDstate)
	{
		case init:
			break;
		case wait_left:
			break;
		case wait_right:
			break;
		case shift_left:
			if( GetBit(PORTC, 0) )
				pattern =  pattern << 1;
			break;
		case shift_right:
			if( GetBit(PORTC, 4) )
				pattern = pattern >> 1;
			break;
		default:
			break;
	}
			transmit_data(pattern);		// Pattern to display
			transmit_data1(row);		// Row(s) displaying pattern
}
void Shift_Task()
{
	LEDstate = init;
	for(;;)
	{
		Demo_Tick();
		vTaskDelay(250);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Shift_Task, (signed portCHAR *)"Shift_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	StartShiftPulse(1);
	vTaskStartScheduler();
	return 0;	
}

/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #7  Exercise #4
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

unsigned char pattern = 0x00;
unsigned char row = 0xF5;
enum LRStates{init, left, right} LRstate;
enum UDStates{wait, up, down} UDstate;
enum LEDMatrixStates{display} LEDstate;


// Pins on PORTA are used as input for A2D conversion
// The Default Channel is 0 (PA0)
// The value of pinNum determines the pin on PORTA used for A2D conversion
// Valid values range between 0 and 7, where the value represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum)
{
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for(i = 0; i < 15; ++i){asm("nop");}
}
void A2D_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//ADEN: Enables analog-to-digital conversion
	//ADSC: Starts analog-to-digital conversion
	//ADATE: Enables auto-triggering, allowing for constant analog to digital conversions.
}
void transmit_data(unsigned char data)
{
	unsigned char i;
	for(i = 0; i < 8; ++i)
	{
		PORTC = 0x08;
		PORTC |= ((data >> i) & 0x01);
		PORTC |= 0x04;
	}
	PORTC |= 0x02;
	PORTC = 0x00;
}
void transmit_data1(unsigned char data)
{
	unsigned char i;
	for(i = 0; i < 8; ++i)
	{
		PORTD = 0x08;
		PORTD |= ((data >> i) & 0x01);
		PORTD |= 0x04;
	}
	PORTD |= 0x02;
	PORTD = 0x00;
}
void LEDMatrix_Tick()
{
	switch(LEDstate)
	{
		case display:
			break;
		default:
			break;
	}
	switch(LEDstate)
	{
		case display:

			transmit_data(pattern);
			transmit_data1(row);
			break;
		default:
			break;
	}
}
void LR_Tick()
{
	switch(LRstate)
	{
		case init:
			if(ADC > 700)
				LRstate = right;
			else if(ADC < 350)
				LRstate = left;
			else
				LRstate = init;
			break;
		case left:
			if(ADC < 350)
				LRstate = left;
			else
				LRstate = init;
			break;
		case right:
			if(ADC > 700)
				LRstate = right;
			else
				LRstate = init;
			break;
		default:
			break;
	}
	switch(LRstate)
	{
		case init:
			break;
		case left:
			if(pattern != 0x80)
				pattern = pattern << 1;
			break;
		case right:
			if(pattern != 0x01)
				pattern = pattern >> 1;
			break;
		default:
			break;
	}
}
void UD_Tick()
{
	switch(UDstate)
	{
		case wait:
			if(ADC > 700)
				UDstate = up;
			else if(ADC < 350)
				UDstate = down;
			else
				UDstate = wait;
			break;
		case up:
			if(ADC > 700)
				UDstate = up;
			else
				UDstate = wait;
			break;
		case down:
			if(ADC < 350)
				UDstate = down;
			else
				UDstate = wait;
			break;
		default:
			break;
	}
	switch(UDstate)
	{
		case wait:
			break;
		case up:
			if(row != 0xFE)
				row = row << 1;
			break;
		case down:
			if(row != 0xE0)
				row = row >> 1;
			break;
		
	}
}
void LEDMatrix_Task()
{
	LEDstate = display;
	for(;;)
	{
		LEDMatrix_Tick();
		vTaskDelay(250);
	}
}
void UD_Task()
{
	UDstate = wait;
	for(;;)
	{
		UD_Tick();
		vTaskDelay(250);
	}
}
void LR_Task()
{
	LRstate = init;
	for(;;)
	{
		LR_Tick();
		vTaskDelay(250);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(UD_Task, (signed portCHAR *) "UD_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(LEDMatrix_Task, (signed portCHAR *) "LEDMatrix_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(LR_Task, (signed portCHAR *) "LR_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	A2D_init();
	StartShiftPulse(1);
	vTaskStartScheduler();
	return 0;
}
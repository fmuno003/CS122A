/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #7  Exercise #1 
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
#include "bit.h"

unsigned short temp = 0;
enum DisplayStates{init} state;
void A2D_init() 
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}
void convert()
{
	ADCSRA |=(1<<ADSC);//start ADC conversion
	while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
	
}
void Display_Tick()
{
	switch(state)
	{
		case init:
			state = init;
			break;
		default:
			break;
	}
	switch(state)
	{
		case init:
			convert();
			temp = ADC;
			PORTC = temp;
			PORTD = temp >> 8;
			break;
		default:
			break;
	}
}
void A2D_Task()
{
	state = init;
	for(;;)
	{
		Display_Tick();
		vTaskDelay(100);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(A2D_Task, (signed portCHAR *)"A2D_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	A2D_init();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	StartShiftPulse(1);
	vTaskStartScheduler();
}

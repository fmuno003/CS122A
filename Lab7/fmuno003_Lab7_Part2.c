/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #7  Exercise #2 
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

unsigned short joystick = 0x0000;
unsigned char pattern = 0x01;
enum JoystickStates{wait, left, right} state;
enum LEDMatrixStates{display} LEDstate;
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
void JoyStick_Tick()
{
	convert();
	joystick = ADC;
	switch(state)
	{
		case wait:
			if(joystick < 350)
				state = left;
			else if(joystick > 700)
				state = right;
			else
				state = wait;
			break;
		case left:
			if(joystick < 350)
				state = left;
			else
				state = wait;
			break;
		case right:
			if(joystick > 700)
				state = right;
			else
				state = wait;
			break;
		default:
			break;
	}
	switch(state)
	{
		case wait:
			break;
		case left:
			if(pattern == 0x80)
				pattern = 0x01;
			else
				pattern = pattern << 1;
			break;
		case right:
			if(pattern == 0x01)
				pattern = 0x80;
			else
				pattern = pattern >> 1;
			break;
	}
}
void JoyStick_Task()
{
	state = wait;
	for(;;)
	{
		JoyStick_Tick();
		vTaskDelay(250);
	}
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
			PORTD = pattern;
			PORTC = ~0x01;
			break;
		default:
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
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(JoyStick_Task, (signed portCHAR *) "JoyStick_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(LEDMatrix_Task, (signed portCHAR *) "LEDMatrix_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
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

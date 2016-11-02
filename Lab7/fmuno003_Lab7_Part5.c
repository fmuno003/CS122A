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

unsigned char pattern = 0x01;
unsigned char row = 0x10;
unsigned short joystick = 0;
unsigned short temp = 0;
unsigned int speeder = 1000;
enum LRStates{leftRight, upDown} state;
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
void convert()
{
	ADCSRA |=(1<<ADSC);//start ADC conversion
	while ( !(ADCSRA & (1<<ADIF)));//wait till ADC conversion
	
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
			PORTC = ~row;
			break;
		default:
			break;
	}
}
void LR_Tick()
{
	switch(state)
	{
		case leftRight:
			Set_A2D_Pin(0x00);
			convert();
			joystick = ADC;
			if(((joystick > 700) && (joystick < 750)) || ((joystick < 450) && (joystick > 350)))
				speeder = 1000;
			else if(((joystick > 750) && (joystick < 850)) || ((joystick < 350) && (joystick > 250)))
				speeder = 500;
			else if(((joystick > 850) && (joystick < 925)) || ((joystick < 250) && (joystick > 175)))
				speeder = 250;
			else if((joystick > 925) || (joystick < 175))
				speeder = 100;
			if(joystick < 350)
			{
				if(pattern != 0x01)
					pattern = pattern >> 1;
			}
			else if(joystick > 700)
			{
				if(pattern != 0x80)
					pattern = pattern << 1;
			}
			state = upDown;
			break;
		case upDown:
			Set_A2D_Pin(0x01);
			convert();
			temp = ADC;
			if(((temp > 700) && (temp < 750)) || ((temp < 450) && (temp > 350)))
				speeder = 1000;
			else if(((temp > 750) && (temp < 850)) || ((temp < 350) && (temp > 250)))
				speeder = 500;
			else if(((temp > 850) && (temp < 925)) || ((temp < 250) && (temp > 175)))
				speeder = 250;
			else if((temp > 925) || (temp < 175))
				speeder = 100;
			if(temp < 350)
			{
				if(row != 0x10)
					row = row << 1;
			}
			else if(temp > 700)
			{
				if(row != 0x01)
					row = row >> 1;
			}
			state = leftRight;
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
		vTaskDelay(100);
	}
}
void LR_Task()
{
	state = leftRight;
	for(;;)
	{
		LR_Tick();
		vTaskDelay(speeder);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LR_Task, (signed portCHAR *) "LR_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(LEDMatrix_Task, (signed portCHAR *) "LEDMatrix_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	A2D_init();
	StartShiftPulse(1);
	vTaskStartScheduler();
	return 0;
}

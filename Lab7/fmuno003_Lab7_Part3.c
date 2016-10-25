/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #7  Exercise #3 
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

unsigned int speeder = 1000;
unsigned short joystick = 0x0000;
unsigned char pattern = 0x00;
enum JoystickStates{wait, left, right} state;
enum LEDMatrixStates{display} LEDstate;

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
void A2D_init()
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}
void JoyStick_Tick()
{
	joystick = ADC;
	switch(state)
	{
		case wait:
			if((joystick > 350) && (joystick < 400))
			{
				speeder = 1000;
				state = left;
			}
			else if((300 < joystick) && (joystick < 350))
			{
				speeder = 500;
				state = left;
			}
			else if((200 < joystick) && (joystick < 300))
			{
				speeder = 250;
				state = left;
			}
			else if(joystick < 200)
			{
				speeder = 100;
				state = left;
			}
			else if((joystick < 700) && (joystick > 650))
			{
				speeder = 1000;
				state = right;
			}
			else if((750 > joystick) && (joystick > 700))
			{
				speeder = 500;
				state = right;
			}
			else if((900 > joystick) && (joystick > 750))
			{
				speeder = 250;
				state = right;
			}
			else if(joystick > 900)
			{
				speeder = 100;
				state = right;
			}
			else
				state = wait;
			break;
		case left:
			if((joystick > 350) && (joystick < 400))
			{
				speeder = 1000;
				state = left;
			}
			else if((300 < joystick) && (joystick < 350))
			{
				speeder = 500;
				state = left;
			}
			else if((200 < joystick) && (joystick < 300))
			{
				speeder = 250;
				state = left;
			}
			else if(joystick < 200)
			{
				speeder = 100;
				state = left;
			}
			else
				state = wait;
			break;
		case right:
			if((joystick < 700) && (joystick > 650))
			{
				speeder = 1000;
				state = right;
			}
			else if((750 > joystick) && (joystick > 700))
			{
				speeder = 500;
				state = right;
			}
			else if((900 > joystick) && (joystick > 750))
			{
				speeder = 250;
				state = right;
			}
			else if(joystick > 900)
			{
				speeder = 100;
				state = right;
			}
			else
				state = wait;
			break;
		default:
			break;
	}
	switch(state)
	{
		case wait:
			speeder = 1000;
			break;
		case left:
			if(pattern == 0x80)
				pattern = 0x01;
			else
				pattern = pattern << 1;
			break;
		case right:
			if(pattern == 0x01)
				pattern = 0x01;
			else
				pattern = pattern << 1;
		break;
	}
}
void JoyStick_Task()
{
	state = wait;
	for(;;)
	{
		JoyStick_Tick();
		vTaskDelay(speeder);
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
		transmit_data(pattern);
		PORTD = ~0x01;
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
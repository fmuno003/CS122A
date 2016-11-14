/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #8  Exercise #2
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

int numPhases = 0;
unsigned char direction = 0;
enum motorStates{A, AB, B, BC, C, CD, D, DA} state;
enum ButtonStates{wait, read} btnState;
void Motor_Tick()
{
	switch(state)
	{
		case A:
			if(!direction && numPhases > 0)
			{
				state = AB;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = DA;
				numPhases--;
			}
			else 
				state = A;
			break;
		case AB:
			if(!direction && numPhases > 0)
			{
				state = B;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = A;
				numPhases--;
			}
			else
				state = AB;
			break;
		case B:
			if(!direction && numPhases > 0)
			{
				state = BC;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = AB;
				numPhases--;
			}
			else
				state = B;
			break;
		case BC:
			if(!direction && numPhases > 0)
			{
				state = C;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = B;
				numPhases--;
			}
			else
				state = BC;
			break;
		case C:
			if(!direction && numPhases > 0)
			{
				state = CD;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = BC;
				numPhases--;
			}
			else
				state = C;
			break;
		case CD:
			if(!direction && numPhases > 0)
			{
				state = D;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = C;
				numPhases--;
			}
			else
				state = CD;
			break;
		case D:
			if(!direction && numPhases > 0)
			{
				state = DA;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = CD;
				numPhases--;
			}
			else
				state = D;
			break;
		case DA:
			if(!direction && numPhases > 0)
			{
				state = A;
				numPhases--;
			}
			else if(direction && numPhases > 0)
			{
				state = D;
				numPhases--;
			}
			else
				state = DA;
			break;
		default:
			break;
	}
	switch(state)
	{
		case A:
			PORTA = 0x01;
			break;
		case AB:
			PORTA = 0x03;
			break;
		case B:
			PORTA = 0x02;
			break;
		case BC:
			PORTA = 0x06;
			break;
		case C:
			PORTA = 0x04;
			break;
		case CD:
			PORTA = 0x0C;
			break;
		case D:
			PORTA = 0x08;
			break;
		case DA:
			PORTA = 0x09;
			break;
		default:
			break;
	}
}
void Button_Tick()
{
	switch(btnState)
	{
		case wait:
			if(GetBit(PINB, 0))
			{
				numPhases = 1024;
				direction = 0;
				btnState = read;
			}
			else if(GetBit(PINB, 1))
			{
				numPhases = 2048;
				direction = 0;
				btnState = read;
			}
			else if(GetBit(PINB, 2))
			{
				numPhases = 4096;
				direction = 0;
				btnState = read;
			}
			else if(GetBit(PINB, 3))
			{
				numPhases = 1024;
				direction = 1;
				btnState = read;
			}
			else if(GetBit(PINB, 4))
			{
				numPhases = 2048;
				direction = 1;
				btnState = read;
			}
			else if(GetBit(PINB, 5))
			{
				numPhases = 4096;
				direction = 1;
				btnState = read;
			}
			else
				btnState = wait;
			break;
		case read:
			if(GetBit(PINB, 0))
				btnState = read;
			else if(GetBit(PINB, 1))
				btnState = read;
			else if(GetBit(PINB, 2))
				btnState = read;
			else if(GetBit(PINB, 3))
				btnState = read;
			else if(GetBit(PINB, 4))
				btnState = read;
			else if(GetBit(PINB, 5))
				btnState = read;
			else
				btnState = wait;
		default:
			break;
	}
	switch(btnState)
	{
		case wait:
			break;
		case read:
			break;
		default:
			break;
	}
}
void Button_Task()
{
	btnState = wait;
	for(;;)
	{
		Button_Tick();
		vTaskDelay(20);
	}
}
void Motor_Task()
{
	state = A;
	for(;;)
	{
		Motor_Tick();
		vTaskDelay(20);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Motor_Task, (signed portCHAR *) "Motor_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(Button_Task, (signed portCHAR *) "Button_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	StartShiftPulse(1);
	vTaskStartScheduler();
}

/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #8  Exercise #1
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
enum motorStates{A, AB, B, BC, C, CD, D, DA} state;
	
void Motor_Tick()
{
	switch(state)
	{
		case A:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = A;
			else if(GetBit(PINB, 0))
				state = DA;
			else if(GetBit(PINB, 1))
				state = B;
			break;
		case AB:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = AB;
			else if(GetBit(PINB, 0))
				state = A;
			else if(GetBit(PINB, 1))
				state = B;
			break;
		case B:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = B;
			else if(GetBit(PINB, 0))
				state = AB;
			else if(GetBit(PINB, 1))
				state = BC;
			break;
		case BC:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = BC;
			else if(GetBit(PINB, 0))
				state = B;
			else if(GetBit(PINB, 1))
				state = C;
			break;			
		case C:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = C;
			else if(GetBit(PINB, 0))
				state = BC;
			else if(GetBit(PINB, 1))
				state = CD;
			break;
		case D:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = D;
			else if(GetBit(PINB, 0))
				state = CD;
			else if(GetBit(PINB, 1))
				state = DA;
			break;
		case DA:
			if((GetBit(PINB, 0) && GetBit(PINB, 1)) || (!GetBit(PINB, 0) && !GetBit(PINB, 1)))
				state = DA;
			else if(GetBit(PINB, 0))
				state = D;
			else if(GetBit(PINB, 1))
				state = A;
			break;
		default:
			break;
	}
	switch(state)
	{
		case A:
			PORTA = 0x11;
			break;
		case AB:
			PORTA = 0x33;
			break;
		case B:
			PORTA = 0x22;
			break;
		case BC:
			PORTA = 0x66;
			break;
		case C:
			PORTA = 0x44;
			break;
		case CD:
			PORTA = 0xCC;
			break;
		case D:
			PORTA = 0x88;
			break;
		case DA:
			PORTA = 0x99;
			break;
		default:
			break;
	}
}
void Motor_Task()
{
	state = A;
	for(;;)
	{
		Motor_Tick();
		vTaskDelay(200);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Motor_Task, (signed portCHAR *) "Motor_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	StartShiftPulse(1);
	vTaskStartScheduler();
}
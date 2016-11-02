/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #6  Exercise #4 
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

unsigned int j;
unsigned int downpress = 1;
unsigned int uppress = 1;
static unsigned char p;
static unsigned char r;
unsigned char pattern[4] = {0x20, 0x10, 0x08, 0x04};
unsigned char row[4] = {0xF1, 0xF5, 0xF5, 0xF1};
enum DemoStates{init, s1, s2, s3, s4} LEDstate;
enum readBtn{btn, left, right, up, down, hold_left,hold_right, hold_up, hold_down} btnState;

void readBtn_Tick()
{
	switch(btnState) 
	{
		case btn:
			if(!GetBit(PINA,0))
				btnState = hold_up;
			else if(!GetBit(PINA,1))
				btnState = hold_down;
			else if(!GetBit(PINA,2))
				btnState = hold_right;
			else if(!GetBit(PINA,3))
				btnState = hold_left;
			else
				btnState = btn;
			break;
		case hold_up:
			if(!GetBit(PINA,0))
				btnState = hold_up;
			else if(uppress != 0)
				btnState = up;
			else 
				btnState = btn;
			break;
		case hold_down:
			if(!GetBit(PINA,1))
				btnState = hold_down;
			else if(downpress != 0)
				btnState = down;
			else
				btnState = btn;
			break;
		case hold_right:
			if(!GetBit(PINA,2))
				btnState = hold_right;
			else 
				btnState = right;
			break;
		case hold_left:
			if(!GetBit(PINA,3))
				btnState = hold_left;
			else
				btnState = left;
			break;
		case up:
			btnState = btn;
			break;
		case down:
			btnState = btn;
			break;
		case right:
			btnState = btn;
			break;
		case left:
			btnState = btn;
			break;
		default:
			btnState = btn;
			break;
	}
	switch(btnState) 
	{
		case btn:
			break;
		case hold_up:
			break;
		case hold_down:
			break;
		case hold_right:
			break;
		case hold_left:
			break;
		case left:
			if( pattern[0] != 0x80 ) 
			{
				for(j = 0; j < 4; j++) 
				{
					pattern[j] = pattern[j] << 1;
				}
			}
			break;
		case right:
			if( pattern[3] != 0x01 ) 
			{
				for(j = 0; j < 4; j++) 
				{
					pattern[j] = pattern[j] >> 1;
				}
			}
			break;
		case up:
			if( uppress != 0 ) 
			{
				for(j = 0; j < 4; j++) 
				{
					row[j] = ~(~row[j] >> 1);
				}
			}
			++downpress;
			--uppress;
			break;
		case down:
			if( downpress != 0) 
			{
				for(j = 0; j < 4; j++) 
				{
					row[j] = ~(~row[j] << 1);
				}
			}
			++uppress;
			--downpress;
			break;
		default:
			break;
	}
}
void Demo_Tick()
{
	switch(LEDstate)
	{
		case init:
			LEDstate = s1;
			break;
		case s1:
			LEDstate = s2;
			break;
		case s2:
			LEDstate = s3;
			break;
		case s3:
			LEDstate = s4;
			break;
		case s4:
			LEDstate = s1;
			break;
		default:
			LEDstate = init;
			break;
		
	}
	switch(LEDstate)
	{
		case init:
			break;
		case s1:
			p = pattern[0];
			r = row[0];
			break;
		case s2:
			p = pattern[1];
			r = row[1];
			break;
		case s3:
			p = pattern[2];
			r = row[2];
			break;
		case s4:
			p = pattern[3];
			r = row[3];
			break;
		default:
			break;
	}
			PORTD = p;		// Pattern to display
			PORTC = r;		// Row(s) displaying pattern
}
void LED_Task()
{
	LEDstate = init;
	for(;;)
	{
		Demo_Tick();
		vTaskDelay(1);
	}
}
void BTN_Task()
{
	btnState = btn;
	for(;;)
	{
		readBtn_Tick();
		vTaskDelay(100);
	}
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(LED_Task, (signed portCHAR *)"LED_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
	xTaskCreate(BTN_Task, (signed portCHAR *)"BTN_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
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

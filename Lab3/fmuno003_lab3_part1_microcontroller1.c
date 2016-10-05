/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #1 
 *	Exercise Description: 
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <stdint.h>
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
#include "usart_ATmega1284.h"

unsigned char temp;
enum Follower_States {on} state;
void Follower_Tick() 
{
	switch(state) 
	{
		case on:
			state = on;
			break;
		default:
			state = on;
			break;
	}
	switch(state) 
	{
		case on:
			if( USART_HasReceived(0) ) 
			{
				temp = USART_Receive(0);
				PORTA = temp;
				USART_Flush(0);
			}
			break;
		default:
			break;
	}
}
void Follower_Task()
{
	state = on;
	for(;;)
	{
		Follower_Tick();
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Follower_Task, (signed portCHAR *)"Follower_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	//TODO:: Please write your application code
	initUSART(0);
	
	// initialize ports
	DDRA = 0xFF; PORTA = 0x00;
	
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	
	return 0;
}
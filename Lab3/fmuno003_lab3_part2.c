/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #2 
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
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "usart_ATmega1284.h"

unsigned char follower = 0;
unsigned char temp;
enum Follower_States {wait, on} state;
enum Leader_States {wait1, on1, off1} state;

void Leader_Tick() 
{
	switch(state) 
	{
		case on1:
			state = off1;
			break;
		case off1:
			state = on1;
			break;
		default:
			state = on1;
            break;
	}
	switch(state) 
	{
		case on:
			if( USART_IsSendReady(0) ) 
			{
				USART_Send(0x01,0);
                PORTA = 0x01;
				USART_Flush(0);
			}
			break;
		case off:
			if( USART_IsSendReady(0) ) 
			{
				USART_Send(0x00,0);
                PORTA = 0x00;
				USART_Flush(0);
			}
            break;
		default:
			break;
	}
}
void Leader_Task()
{
	state = on;
	for(;;)
	{
		Leader_Tick();
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Leader_Task, (signed portCHAR *)"Leader_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
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
void StartSecPulse1(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Follower_Task, (signed portCHAR *)"Follower_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
		//TODO:: Please write your application code
        initUSART(0);
        
        // initialize ports
        DDRA = 0xFF; PORTA = 0x00;
        DDRB = 0x00; PORTB = 0xFF;
		DDRC = 0xFF; PORTC = 0x00;
		
		while(1)
		{
			if(follower == 0)
			{
				PORTC = 0x01;
				//Start Tasks
				StartSecPulse(1);
				//RunSchedular
				vTaskStartScheduler();
			}
			else
			{
				PORTC = 0x00;
				//Start Tasks
				StartSecPulse1(1);
				//RunSchedular
				vTaskStartScheduler();
			}
			if(PORTB == 0x01 && follower == 0)
			{
				follower = 0x01;				
			}
			else if(PORTB == 0x01 && follower == 1)
			{
				follower = 0x00;
			}
		}

    return 0;
}
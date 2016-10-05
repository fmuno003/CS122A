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
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "usart_ATmega1284.h"

enum Leader_States {on,off} state;
void Leader_Tick() 
{
	switch(state) 
	{
		case on:
			state = off;
			break;
		case off:
			state = on;
			break;
		default:
			state = on;
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
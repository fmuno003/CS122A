/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #3  Exercise #3 
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

unsigned char follower = 1;
unsigned char time = 0;
unsigned char temp;
enum Follower_States {wait, on} state;
enum Leader_States {wait1, on1, off1} state1;

void Leader_Tick() 
{
	switch(state) 
	{
    case wait1:
      if(!follower)
      {
        state1 = wait1;
      }
      else
      {
        state1 = on1;
      }
      break;
		case on1:
      if(!follower)
      {
			  state1 = off1;
      }
      else
      {
        state1 = wait1;
      }  
			break;
		case off1:
      if(!follower)
      {
        state1 = on1;
      }
      else
      {
        state1 = wait1;
      }
			break;
		default:
			state1 = wait1;
            break;
	}
	switch(state1) 
	{
    case wait1:
      break;
		case on:
      PORTC = 0x01;
			if( USART_IsSendReady(1) ) 
			{
				USART_Send(0x01,1);
                PORTA = 0x01;
				USART_Flush(1);
			}
			break;
		case off:
			if( USART_IsSendReady(1) ) 
			{
				USART_Send(0x00,1);
                PORTA = 0x00;
				USART_Flush(1);
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
    case wait:
      if(!USART_HasReceived(0) && time >= 30)
      {
        state = wait;
        follower = 0;
      }
      else
      {
        state = on;
        follower = 1;
      }
      break;
		case on:
      if (time < 30)
      {
        state = on;
      }
      else
      {
        state = wait;
      }
      break;
		default:
		  state = wait;
		  break;
	}
	switch(state)
	{
    case wait:
      break;
		case on:
      PORTC = 0x00;
		if( USART_HasReceived(0) )
		{
			temp = USART_Receive(0);
			PORTA = temp;
			USART_Flush(0);
      j = 0;
		}
    else
    {
      j++;
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
        initUSART(1);
        
        // initialize ports
        DDRA = 0xFF; PORTA = 0x00;
        DDRB = 0x00; PORTB = 0xFF;
		    DDRC = 0xFF; PORTC = 0x00;
		
				//Start Tasks
				StartSecPulse1(1);
				//RunSchedular
				vTaskStartScheduler();

    return 0;
}

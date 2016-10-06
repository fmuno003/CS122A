/*	Partner(s) Name & E-mail: Francisco Munoz
 *	Lab Section: 022
 *	Assignment: Lab #5  Exercise #1 
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

void transmit_data(unsigned char data) 
{
	unsigned char i;
	for (i = 0; i < 8 ; ++i) 
  {
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the “Shift” register to the “Storage” register
	PORTC |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
}

unsigned char temp = 0x00;
unsigned char toggler = 0x00;
enum Shift_Registers {increment, decrement, wait, pause, toggle} state;
void Shift_Register_Tick()
{
    switch(state)
    {
        case wait:
            if(~PINA & 0x01 == 0x01)
            {
                state = increment;
            }
            else if (~PINA & 0x02 == 0x02)
            {
                state = decrement;
            }
            else if(~PINA & 0x03 == 0x03)
            {
              state = toggle;
            }
            break;
       case increment:
            if(~PINA & 0x01 == 0x01)
            {
                state = pause;
            }
            else
            {
                state = wait;
            }
            break;
       case decrement:
            if(~PINA & 0x02 == 0x02)
            {
              state = pause;
            }
            else
            {
              state = wait;
            }
            break;
       case pause:
            if(~PINA == 0x00)
            {
              state = wait;
            }
            break;
        case toggle:
            state = wait;
            break;
        default:
            break;
    }
    switch(state):
        case wait:
            break;
        case increment:
            if(temp != 0xFF)
            {
               temp++;
               transmit_data(temp);
            }
            break;
        case decrement:
            if(temp != 0x00)
            {
              temp--;
              transmit_data(temp);
            }
        case pause:
            break;
        case toggle:
            if(toggler == 0)
            {
                toggler = 1;
                transmit_data(temp);
            }
            else
            {
                toggler = 0;
                transmit_data(toggler);
             }
             break;
        default:
            break;
}
void Shift_Task()
{
    state = wait;
    for(;;)
    {
         Shift_Register_Tick();
    }
}
void StartShiftPulse(unsigned portBASE_TYPE Priority)
{
    xTaskCreate(Shift_Task, (signed portCHAR *)"SHIFT_TASK", configMINIMAL_STACK_SIZE, NULL, Priority, NULL);
}
int main()
{
    DDRA = 0xFF; PORTA = 0x00;
    DDRC = 0x00; PORTC = 0xFF;
    
    StartShiftPulse(1);
    vStartScheduler();
    return 0;
}

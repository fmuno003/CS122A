/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: Fix Code
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
#include "bit.h"

int count = 0;
unsigned char receivedData;
enum servantStates {s_wait, read} states;
enum masterStates {wait, send} state;

void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	DDRB = 0xBF; PORTB = 0x40;
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	//   of fosc/16  (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	SREG =0x80;
}
void SPI_MasterTransmit(unsigned char cData) {
	// set SS low
	PORTB = SetBit(PORTB,4,0);
	// data in SPDR will be transmitted, e.g. SPDR = cData;
	SPDR = cData;
	while(!(SPSR & (1<<SPIF))) { // wait for transmission to complete
		;
	}
	// set SS high
	PORTB = SetBit(PORTB,4,1);
}
void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x40; PORTB = 0xBF;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= (1<<SPE) | (1<<SPIE);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}
void TickFct_master() 
{
	switch( state ) 
	{
		case wait:
			state = send;
			break;
		case send:
			state = wait;
			break;
		default:
			break;
	}
	switch( state ) 
	{
		case wait:
			PORTA = SetBit(PORTA,2,0);
			count++;
			if( count >= 0xFF )
				count = 0;
			break;
		case send:
			PORTA = SetBit(PORTA,2,1);
			SPI_MasterTransmit(count);
			break;
		default:
			break;
	}
}
void TickFct_servant() 
{
	switch( states )  
	{
		case s_wait:
			states = read;
			break;
		case read:
			states = s_wait;
			break;
		default:
			states = s_wait;
			break;
	}
	switch( states ) 
	{
		case s_wait:
			PORTA = SetBit(PORTA,0,0);
			break;
		case read:
			PORTA = SetBit(PORTA,0,1);
			PORTC = receivedData;
			break;
		default:
			break;
	}
}
void Servant_Task()
{
	state = wait;
	for(;;)
	{
		TickFct_servant();
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Master_Task, (signed portCHAR *)"Master_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
        // MASTER
        /*DDRC = 0xF0; PORTC = 0x0F;
        DDRD = 0xFF; PORTD = 0x00;
        DDRA = 0xFF; PORTA = 0x00;
        SPI_MasterInit();*/
        
        //SLAVE
        SPI_ServantInit();
        DDRC = 0xFF; PORTC = 0x00;
        DDRA = 0xFF; PORTA = 0x00;
		StartSecPulse(1);
		vTaskStartScheduler();
		
	return 0;       
}

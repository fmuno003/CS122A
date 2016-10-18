/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: Servant
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

unsigned char receivedData;
enum servantStates {s_wait, read} states;
void transmit_data(unsigned char data) {
	unsigned char i;
	for (i = 0; i < 8 ; ++i) {
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
void SPI_ServantInit(void) {
	// set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	DDRB = 0x40; PORTB = 0xBF;
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	SPCR |= (1<<SPE) | (1<<SPIE);
	// make sure global interrupts are enabled on SREG register (pg. 9)
	SREG = 0x80;
}
ISR(SPI_STC_vect) { // this is enabled in with the SPCR register?s ?SPI
	// Interrupt Enable?
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;
	receivedData = SPDR;
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
			break;
		case read:
			transmit_data(receivedData);
			break;
		default:
			break;
	}
}
void Servant_Task()
{
	states = s_wait;
	for(;;)
	{
		TickFct_servant();
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Servant_Task, (signed portCHAR *)"Servant_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	//SLAVE
	DDRC = 0xFF; PORTC = 0x00;
	SPI_ServantInit();
	StartSecPulse(1);
	vTaskStartScheduler();
	return 0;       
}

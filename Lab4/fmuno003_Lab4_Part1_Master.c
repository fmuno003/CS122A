/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: Master
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

int count = 0xC7;
unsigned char receivedData;
enum masterStates {wait, send} state;

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
			break;
		case send:
			SPI_MasterTransmit(count);
			break;
		default:
			break;
	}
}
void Master_Task()
{
	state = wait;
	for(;;)
	{
		TickFct_master();
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Master_Task, (signed portCHAR *)"Master_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	// MASTER
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;
	SPI_MasterInit();
	StartSecPulse(1);
	vTaskStartScheduler();
	return 0;       
}

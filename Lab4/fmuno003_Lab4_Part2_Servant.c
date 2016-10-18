/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: Servant
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/interrupt.h>
#include <avr/eeprom.h>
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

unsigned char temp, speed, data, pattern, receivedData;
unsigned char pattern3 = 0x04;
enum servantStates {s_wait, read} states;
enum pattern1States {wait, light1, light2, light3, light4, light5, light6, shift_left, shift_right} lightState;

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
void transmit_data(unsigned char data) 
{
	for(int i = 0; i < 8; ++i)
	{
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		PORTC = 0x08;
		// set SER = next bit of data to be sent.
		PORTC |= ((data >> i) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		PORTC |= 0x04;
	}
	// set RCLK = 1. Rising edge copies data from the �Shift� register to the
	// �Storage� register
	PORTC |= 0x02;
	// clears all lines in preparation of a new transmission
	PORTC = 0x00;
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
			temp = receivedData;
			speed = temp & 0x0F;
			pattern = (temp >> 4) & 0x0F;
			break;
		default:
			break;
	}
}
void TickFct_pattern1()
{
	switch(lightState)
	{
		case wait:
			if(pattern == 1)
				lightState = light1;
			else if(pattern == 2)
				lightState = light3;
			else if(pattern == 3)
				lightState = shift_left;
			else if(pattern == 4)
				lightState = light5;
			else
				lightState = wait;
			break;
		case light1:
			if(pattern != 1)
				lightState = wait;
			else if(pattern == 1)
				lightState = light2;
			break;
		case light2:
			if(pattern != 1)
				lightState = wait;
			else if(pattern == 1)
				lightState = light1;
			break;
		case light3:
			if(pattern != 2)
				lightState = wait;
			else if(pattern == 2)
				lightState = light4;
			break;
		case light4:
			if(pattern != 2)
				lightState = wait;
			else if(pattern == 2)
				lightState = light3;
			break;
		case shift_left:
			if(pattern == 3 && pattern3 != 0x80)
				lightState = shift_left;
			else if(pattern == 3 && pattern3 == 0x80)
				lightState = shift_right;
			else if(pattern != 3)
				lightState = wait;
			break;
		case shift_right:
			if(pattern == 3 && pattern3 != 0x01)
				lightState = shift_right;
			else if(pattern == 3 && pattern3 == 0x01)
				lightState = shift_left;
			else if(pattern != 3)
				lightState = wait;
			break;
		case light5:
			if(pattern != 4)
				lightState = wait;
			else if(pattern == 4)
				lightState = light6;
			break;
		case light6:
			if(pattern != 4)
				lightState = wait;
			else if(pattern == 4)
				lightState = light5;
			break;
		default:
			break;
	}
	switch(lightState)
	{
		case wait:
			break;
		case light1:
			transmit_data(0x0F);
			break;
		case light2:
			transmit_data(0xF0);
			break;
		case light3:
			transmit_data(0xAA);
			break;
		case light4:
			transmit_data(0x55);
			break;
		case shift_left:
			pattern3 = pattern3 << 1;
			transmit_data(pattern3);
			break;
		case shift_right:
			pattern3 = pattern3 >> 1;
			transmit_data(pattern3);
			break;
		case light5:
			transmit_data(0xC3);
			break;
		case light6:
			transmit_data(0x3C);
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
		vTaskDelay(200);
	}
}
void Pattern1_Task()
{
	lightState = wait;
	for(;;)
	{
		TickFct_pattern1();
		vTaskDelay(200);
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Servant_Task, (signed portCHAR *)"Servant_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
void StartSecPulse1(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Pattern1_Task, (signed portCHAR *)"Pattern1_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
	//SLAVE
	SPI_ServantInit();
	DDRC = 0xFF; PORTC = 0x00;
	StartSecPulse(1);
	StartSecPulse1(1);
	vTaskStartScheduler();
}

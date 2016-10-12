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
#include "keypad.h"
#include "lcd.h"

unsigned char receivedData;
unsigned char temp;
unsigned char pattern;
unsigned char speed;
unsigned char clr_highNibble = 0x0F;
unsigned char clr_lowNibble = 0xF0;
unsigned char btn;
unsigned char data = 0x11;
unsigned char pattern3 = 0x01;
unsigned char pattern4 = 0xF0;

enum servantStates {s_wait, read} states;
enum masterStates {m_read} state;
enum pattern1States {wait1, light1, light2} lightState;
enum pattern2States {wait2, light3, light4} lightState2;
enum pattern3States {wait3, shift_left, shift_right} lightState3;
enum pattern4States {wait4, shift_left4, shift_right4} lightState4;
enum LCDStates {LCD_read} LCDState;
		
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
ISR(SPI_STC_vect){ // this is enabled in with the SPCR register?s ?SPI
	// Interrupt Enable?
	// SPDR contains the received data, e.g. unsigned char receivedData =
	// SPDR;
	receivedData = SPDR;
}
void TickFct_master() 
{
	switch( state ) 
	{
		case m_read:
			state = m_read;
			break;
		default:
			break;
	}
	switch( state ) 
	{
		case m_read:
			btn = GetKeypadKey();
		switch( btn ) 
		{
			case 'A':
				data = data & clr_highNibble;
				data = data | 0x10;
				PORTA = SetBit(PORTA,2,1);
				break;
			case 'B':
				data = data & clr_highNibble;
				data = data | 0x20;
				PORTA = SetBit(PORTA,2,1);
				break;
			case 'C':
				data = data & clr_highNibble;
				data = data | 0x30;
				PORTA = SetBit(PORTA,2,1);
				break;
			case 'D':
				data = data & clr_highNibble;
				data = data | 0x40;
				PORTA = SetBit(PORTA,2,1);
				break;
			case '1':
				data = data & clr_lowNibble;
				data = data | 0x01;
				PORTA = SetBit(PORTA,2,1);
				break;
			case '2':
				data = data & clr_lowNibble;
				data = data | 0x02;
				PORTA = SetBit(PORTA,2,1);
				break;
			case '3':
				data = data & clr_lowNibble;
				data = data | 0x03;
				PORTA = SetBit(PORTA,2,1);
				break;
			case '4':
				data = data & clr_lowNibble;
				data = data | 0x04;
				PORTA = SetBit(PORTA,2,1);
				break;
			case '5':
				data = data & clr_lowNibble;
				data = data | 0x05;
				PORTA = SetBit(PORTA,2,1);
				break;
			case '6':
				data = data & clr_lowNibble;
				data = data | 0x06;
				PORTA = SetBit(PORTA,2,1);
				break;
			default:
				PORTA = SetBit(PORTA,2,0);
				break;
		}
		SPI_MasterTransmit(data);
		break;
		default:
			break;
	}
}
void TickFct_servant() 
{
	switch( state )  
	{
		case s_wait:
			state = read;
			break;
		case read:
			state = s_wait;
			break;
		default:
			state = s_wait;
			break;
	}
	switch( state ) 
	{
		case s_wait:
			PORTA = SetBit(PORTA,0,0);
			break;
		case read:
			PORTA = SetBit(PORTA,0,1);
			temp = receivedData;
			speed = temp & 0x0F;
			pattern = (temp >> 4) & 0x0F;
			break;
		default:
			break;
	}
}
void TickFct_pattern1() {
	switch( lightState ) 
	{
		case wait1:
			if(pattern == 1)
				lightState = light1;
			else
				lightState = wait1;
			break;
		case light1:
			if(pattern != 1)
				lightState = wait1;
			else if(pattern == 1)
				lightState = light2;
			break;
		case light2:
			if(pattern != 1)
				lightState = wait1;
			else if(pattern == 1)
				lightState = light1;
			break;
		default:
			break;
	}
	switch( lightState ) 
	{
		case wait1:
			break;
		case light1:
			PORTC = pattern | 0x0E;
			break;
		case light2:
			PORTC = 0xF0;
			break;
		default:
			break;
	}
}
void TickFct_pattern2() 
{
	switch( lightState2 ) 
	{
		case wait2:
			if(pattern == 2)
				lightState2 = light3;
			else
				lightState2 = wait2;
			break;
		case light3:
			if(pattern != 2)
				lightState2 = wait2;
			else if(pattern == 2)
				lightState2 = light4;
			break;
		case light4:
			if(pattern != 2)
				lightState2 = wait2;
			else if(pattern == 2)
				lightState2 = light3;
			break;
		default:
			break;
	}
	switch( lightState2 ) 
	{
		case wait2:
			break;
		case light3:
			PORTC = pattern | 0xA8;
			break;
		case light4:
			PORTC = 0x55;
			break;
		default:
			break;
	}
}
void TickFct_pattern3() 
{
	switch( lightState3 ) 
	{
		case wait3:
			if(pattern == 3) 
			{
				pattern3 = 0x80;
				lightState3 = shift_right;
			}
			else
				lightState3 = wait3;
			break;
		case shift_left:
			if(pattern == 3 && pattern3 != 0x80)
				lightState3 = shift_left;
			else if(pattern ==3 && pattern3 == 0x80)
				lightState3 = shift_right;
			else if(pattern != 3)
				lightState3 = wait3;
			break;
		case shift_right:
			if(pattern == 3 && pattern3 != 0x01)
				lightState3 = shift_right;
			else if(pattern == 3 && pattern3 == 0x01)
				lightState3 = shift_left;
			else if(pattern != 3)
				lightState3 = wait3;
			break;
		default:
			break;
	}
	switch( lightState3 ) 
	{
		case wait3:
			pattern3 = 0x00;
			break;
		case shift_left:
			pattern3 = pattern3 << 1;
			PORTC = pattern3;
			break;
		case shift_right:
			pattern3 = pattern3 >> 1;
			PORTC = pattern3;
			break;
		default:
			break;
	}
}
void TickFct_pattern4() 
{
	switch( lightState4 ) 
	{
		case wait4:
			if(pattern == 4)
				lightState4 = shift_right4;
			else
				lightState4 = wait4;
			break;
		case shift_left4:
			if(pattern == 4 && pattern4 != 0xF0)
				lightState4 = shift_left4;
			else if(pattern == 4 && pattern4 == 0xF0)
				lightState4 = shift_right4;
			else if(pattern != 4)
				lightState4 = wait4;
			break;
		case shift_right4:
			if(pattern == 4 && pattern4 != 0x0F)
				lightState4 = shift_right4;
			else if(pattern == 4 && pattern4 == 0x0F)
				lightState4 = shift_left4;
			else if(pattern != 4)
				lightState4 = wait4;
			break;
		default:
			break;
	}
	switch( lightState4 ) 
	{
		case wait4:
			break;
		case shift_left4:
			pattern4 = pattern4 << 1;
			PORTC = pattern4;
			break;
		case shift_right4:
			pattern4 = pattern4 >> 1;
			PORTC = pattern4;
			break;
		default:
			break;
	}
}
void TickFct_LCD() 
{
	switch( LCDState ) 
	{
		case LCD_read:
			state = LCD_read;
			break;
		default:
			break;
	}
	switch( LCDState )
	{
		case LCD_read:
			break;
		default:
			break;
	}
}
void Servant_Task()
{
	state = m_read;
	for(;;)
	{
		TickFct_master();
	}
}
void StartSecPulse(unsigned portBASE_TYPE Priority)
{
	xTaskCreate(Servant_Task, (signed portCHAR *)"Servant_Task", configMINIMAL_STACK_SIZE, NULL, Priority, NULL );
}
int main(void)
{
        // MASTER
        DDRC = 0xF0; PORTC = 0x0F;
        DDRD = 0xFF; PORTD = 0x00;
        DDRA = 0xFF; PORTA = 0x00;
        SPI_MasterInit();
        
		// SLAVE
        /*SPI_ServantInit();
        DDRC = 0xFF; PORTC = 0x00;
        DDRA = 0xFF; PORTA = 0x00;*/
		StartSecPulse(1);
		vTaskStartScheduler();
		
	return 0;       
}


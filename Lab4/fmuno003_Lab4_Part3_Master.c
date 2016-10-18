/*	Partner(s) Name & E-mail: Francisco Munoz	fmuno003@ucr.edu
 *	Lab Section: 022
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: Master
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */ 

#include <avr/eeprom.h>
//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
#include "keypad.h"
#include "lcd.h"

char array[22] = "Ptrn: 1 Spd: 1  uC: 1";
unsigned char clr_highNibble = 0x0F;
unsigned char clr_lowNibble = 0xF0;
unsigned char data, btn;

enum masterStates {m_read} state;
void SPI_MasterInit(void) {
	// Set DDRB to have MOSI, SCK, and SS as output and MISO as input
	DDRB = 0xBF; PORTB = 0x40;
	// Set SPCR register to enable SPI, enable master, and use SCK frequency
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);
	//   of fosc/16  (pg. 168)
	// Make sure global interrupts are enabled on SREG register (pg. 9)
	SREG =0x80;
}
void SPI_MasterTransmit(unsigned char cData) 
{
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
					array[6] = '1';
					break;
				case 'B':
					data = data & clr_highNibble;
					data = data | 0x20;
					array[6] = '2';
					break;
				case 'C':
					data = data & clr_highNibble;
					data = data | 0x30;
					array[6] = '3';
					break;
				case 'D':
					data = data & clr_highNibble;
					data = data | 0x40;
					array[6] = '4';
					break;
				case '1':
					data = data & clr_lowNibble;
					data = data | 0x01;
					array[13] = '1';
					break;
				case '2':
					data = data & clr_lowNibble;
					data = data | 0x02;
					array[13] = '2';
					break;
				case '3':
					data = data & clr_lowNibble;
					data = data | 0x03;
					array[13] = '3';
					break;
				case '4':
					data = data & clr_lowNibble;
					data = data | 0x04;
					array[13] = '4';
					break;
				case '5':
					data = data & clr_lowNibble;
					data = data | 0x05;
					array[13] = '5';
					break;
				case '6':
					data = data & clr_lowNibble;
					data = data | 0x06;
					array[13] = '6';
					break;
				default:
					break;
			}
			SPI_MasterTransmit(data);
			LCD_DisplayString(1, array);
			break;
		default:
			break;
	}
}
void Master_Task()
{
	state = m_read;
	for(;;)
	{
		TickFct_master();
		vTaskDelay(250);
		
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
		DDRA = 0xFF; PORTD = 0x00;
		DDRD = 0xFF; PORTD = 0x00;
		LCD_init();
        SPI_MasterInit();
		StartSecPulse(1);
		vTaskStartScheduler();
		
	return 0;       
}

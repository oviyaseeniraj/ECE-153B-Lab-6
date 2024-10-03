/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6B
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;	

int main(void) {
  static uint32_t ComputedCRC;
	uint32_t time;
	
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	SysTick_Init();
	LED_Init();	
	CRC_Init();
	UART2_GPIO_Init();
	UART2_Init();
	USART_Init(USART2);
	
	while(1) {
		// toggle LED
		LED_Toggle();
		// Compute the CRC of DataBuffer
		// start timer
		startTimer();
		// reset CRC
		CRC_Init();
		// compute CRC
		ComputedCRC = CRC_CalcBlockCRC(DataBuffer, BUFFER_SIZE);
		// stop timer
		time = endTimer();
		// if CRC doesn't match expected CRC, turn LED off, break
		 if (ComputedCRC != uwExpectedCRCValue) {
      LED_Off();
      break;
    }
		// print time to compute
		printf("time to calculate: %f ms\r\n", time/1.0);
		// delay 1 second
		delay(1000);
	}
}



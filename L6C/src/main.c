/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */

#include "stm32l476xx.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "LED.h"
#include "CRC.h"
#include "DMA.h"
#include "UART.h"
#include <stdio.h>

/* Expected CRC Value */
static uint32_t uwExpectedCRCValue = 0x5A60861E;	
static volatile uint32_t ComputedCRC;	
static volatile uint32_t ComputationDone = 0;	

void completeCRC(uint32_t crc){
	//TODO
	//store calculated CRC value in a volatile static variable
	ComputedCRC = crc;
	
	//mark computation completed
	ComputationDone = 1;
}

int main(void) {
  	uint32_t time;
	
	// Switch System Clock = 80 MHz
	System_Clock_Init(); 
	// systick init
	SysTick_Init();
	// uart init
	UART2_GPIO_Init();
  UART2_Init();
  USART_Init(USART2);
	// led, crc, dma init
	LED_Init();
	CRC_Init();
	DMA_Init();
	
	while(1) {
		// TODO
		DMA_Init();
		//show calc is running
		LED_Toggle();
		//start timer
		startTimer();
		//mark computation not comoplete
		ComputationDone = 0;
		//reset crc
		CRC->CR = CRC_CR_RESET;
		//number of data to transfer
		DMA1_Channel6->CNDTR = BUFFER_SIZE;
		//start data transfer and CRC computation
		DMA1_Channel6->CCR |= DMA_CCR_EN;
		//wait for computation to be done
		while (!ComputationDone);
		//disable DMA channel
		DMA1_Channel6->CCR &= ~DMA_CCR_EN;
		//stop timer and store time
		time = endTimer();
		if (ComputedCRC != uwExpectedCRCValue)
		{
			LED_Off();
			break;
		}
		printf("time to calculate: %f microseconds\r\n", time/1.0);
		delay(1000);
	}
	
  while (1); 
}



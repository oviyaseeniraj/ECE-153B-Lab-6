/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#include "SysTimer.h"

static uint32_t volatile step;

void SysTick_Init(void) {
	// SysTick Control & Status Register
	SysTick->CTRL = 0; // Disable SysTick IRQ and SysTick Counter
	
	// Enables SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does not assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Select clock source
	// If CLKSOURCE = 0, the external clock is used. The frequency of SysTick clock is the frequency of the AHB clock divided by 8.
	// If CLKSOURCE = 1, the processor clock is used.
	// TODO:
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;	
	
	// Configure and Enable SysTick interrupt in NVIC
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 1); // Set Priority to 1
}

void SysTick_Handler(void) {
	++step;
}

void delay(uint32_t ms) {
    //TODO

    //reset
    step = 0;
    SysTick->VAL = 0;

    SysTick->LOAD = 9999;

  //enable systick
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

  //wait for counter to reach desired value
    while (step < ms) {
        }

  //disable SysTick
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void startTimer(void) {
    //TODO
    //reset
    step = 0;
    SysTick->VAL = 0;

    SysTick->LOAD = 9999;

  //start timer
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

}

uint32_t endTimer(void) {
    //TODO

    //pause timer
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

  //calculate time
  uint32_t time = ((SysTick->LOAD - SysTick->VAL) + SysTick->LOAD * step)/10;
  return time;
}
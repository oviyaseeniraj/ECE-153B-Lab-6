/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6A
 */

#include "UART.h"

void UART1_Init(void) {
	// [TODO]
	RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;//enable usart1 clk
	RCC->CCIPR |= RCC_CCIPR_USART1SEL_0;

}

void UART2_Init(void) {
	// [TODO]
	RCC -> APB1ENR1 |= RCC_APB1ENR1_USART2EN;
	RCC->CCIPR |= RCC_CCIPR_USART2SEL_0;
}

void UART1_GPIO_Init(void) {
	// [TODO]
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	//set pb7 to alt function mode
	GPIOB -> MODER &= ~GPIO_MODER_MODE7_0;
	GPIOB -> MODER |= GPIO_MODER_MODE7_1;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL7_0;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL7_1;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL7_2;
	GPIOB -> AFR[0] &= ~GPIO_AFRL_AFSEL7_3;
	
	//set pb6 to alt function mode
	GPIOB -> MODER &= ~GPIO_MODER_MODE6_0;
	GPIOB -> MODER |= GPIO_MODER_MODE6_1;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL6_0;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL6_2;
	GPIOB -> AFR[0] &= ~GPIO_AFRL_AFSEL6_3;
	
	//set to very high speed
	GPIOB -> OSPEEDR |= GPIO_OSPEEDR_OSPEED6;
	GPIOB -> OSPEEDR |= GPIO_OSPEEDR_OSPEED7;
	
	//set to pull up
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD6;
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPD6_0;
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD7;
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPD7_0;
}

void UART2_GPIO_Init(void) {
	// [TODO]
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	//set pA3 to alt function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODE3_0;
	GPIOA -> MODER |= GPIO_MODER_MODE3_1;
	GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL3_0;
	GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL3_1;
	GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL3_2;
	GPIOA -> AFR[0] &= ~GPIO_AFRL_AFSEL3_3;
	
	//set pA2 to alt function mode
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA -> MODER |= GPIO_MODER_MODE2_1;
	GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL2_0;
	GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL2_1;
	GPIOA -> AFR[0] |= GPIO_AFRL_AFSEL2_2;
	GPIOA -> AFR[0] &= ~GPIO_AFRL_AFSEL2_3;
	
	//set to very high speed
	GPIOA -> OSPEEDR |= GPIO_OSPEEDR_OSPEED2;
	GPIOA -> OSPEEDR |= GPIO_OSPEEDR_OSPEED3;
	
	//set to pull up
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD2;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD2_0;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD3;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD3_0;
}

void USART_Init(USART_TypeDef* USARTx) {
	// [TODO]
	//disable usart
	USARTx -> CR1 &= ~USART_CR1_UE; 
	
	//word length:8, oversampling: 16, stop bit:1
	USARTx -> CR1 &= ~USART_CR1_M;
	USARTx -> CR1 &= ~USART_CR1_OVER8;
	USARTx -> CR2 &= ~USART_CR2_STOP;
	
	//enable transmitter and receiver
	USARTx -> CR1 |= USART_CR1_TE;
	USARTx -> CR1 |= USART_CR1_RE;
	
	//enable usart
	USARTx -> CR1 |= USART_CR1_UE;
	
	//baud rate: 9600
	USARTx -> BRR = 80000000 / 9600;
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}   

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;    
	while(--time);   
}

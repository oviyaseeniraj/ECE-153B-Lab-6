/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 6C
 */
 
#include "DMA.h"
#include "CRC.h"

void DMA_Init(void) {
	//TODO
	
	// Enable DMA clk
  RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
  
  // Wait 20us
  for (volatile uint32_t i = 0; i < 1000; ++i);
  
	//channel 6
  //disable
  DMA1_Channel6->CCR &= ~DMA_CCR_EN;
  
  //memory-to-memory mode
  DMA1_Channel6->CCR |= DMA_CCR_MEM2MEM;
  
  //high priority
  DMA1_Channel6->CCR |= DMA_CCR_PL;
  
  // peripheral size = 32-bit
  DMA1_Channel6->CCR |= DMA_CCR_PSIZE_1;
  
  //memory size = 32-bit
  DMA1_Channel6->CCR |= DMA_CCR_MSIZE_1;
  
  //disable peripheral increment mode
  DMA1_Channel6->CCR &= ~DMA_CCR_PINC;
  
  //memory increment mode
  DMA1_Channel6->CCR |= DMA_CCR_MINC;
  
  //disable circular mode
  DMA1_Channel6->CCR &= ~DMA_CCR_CIRC;
  
  //Memory-to-Peripheral
  DMA1_Channel6->CCR |= DMA_CCR_DIR;
  
  //data source
  DMA1_Channel6->CMAR = (uint32_t)DataBuffer;
  
  //data destination
  DMA1_Channel6->CPAR = (uint32_t)&(CRC->DR);
  
	//dma interrupt
  //disable half transfer interrupt
  DMA1_Channel6->CCR &= ~DMA_CCR_HTIE;
  
  //disable transfer error interrupt
  DMA1_Channel6->CCR &= ~DMA_CCR_TEIE;
  
  //enable transfer complete interrupt
  DMA1_Channel6->CCR |= DMA_CCR_TCIE;
  
  //set interrupt priority to 0 in NVIC
  NVIC_SetPriority(DMA1_Channel6_IRQn, 0);
  
  //enable interrupt in NVIC
  NVIC_EnableIRQ(DMA1_Channel6_IRQn);
}

void DMA1_Channel6_IRQHandler(void){ 
	// TODO
	//clear NVIC interrupt flag
  NVIC_ClearPendingIRQ(DMA1_Channel6_IRQn);
  
  //Transfer Complete interrupt flag
  if ((DMA1->ISR & DMA_ISR_TCIF6) != 0) {
    //clear flag
    DMA1->IFCR = DMA_IFCR_CTCIF6;
    // mark computation completed
    completeCRC(CRC->DR);
  }
  //clear global DMA interrupt flag
  DMA1->IFCR = DMA_IFCR_CGIF6;
}


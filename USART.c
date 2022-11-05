#include "USART.h"

void USART_Send(char c){
	while(!(USART2->ISR & USART_ISR_TC));
	USART2->TDR = c;
}

void USART_putString(char * string){
	while(*string){
		USART_Send(*string);
		string++;
	}
}

void USART_config(uint32_t baudrate){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;						//Clock Enbale GPIOA
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;					//Clock Enbale USART2

	GPIOA->MODER |= (0x02<<4) | (0x02<<30);				//Alternate Function PA2 & PA15
	GPIOA->AFR[0] |= (0x07<<8);									  //PA2 as TX2
	GPIOA->AFR[1] |= (0x07<<28);									//PA15 as RX2
	
	USART2->BRR = (uint32_t)(SystemCoreClock/(baudrate * 2));  //round( 64MHz/115200)
	USART2->CR1 |= USART_CR1_TE + USART_CR1_RE;					// Habiliar recepcion y transmision
	USART2->CR1 |= USART_CR1_RXNEIE;										// Interrupción recepcion
	USART2->CR1 |= USART_CR1_UE;												// Habilitar modulo UART (puerto serial)
	
	NVIC_EnableIRQ(USART2_IRQn);
}

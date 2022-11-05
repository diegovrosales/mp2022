#include "stm32f3xx.h"                  // Device header
#include <stdio.h>
#include "breakout.h"
#include "USART.h"


#define SysTickIHFreq	1000 

uint16_t adc_data;
char str[80];

void ADC_Init(void);
void SysTick_Handler(void);
void clk_config(void);

int main(void) {
	clk_config();
	ADC_Init();
	setup_game();
	
	
	USART_config(300000);
	
	USART_putString("\n\r        Breakout      \n\r");
	
	SysTick_Config(SystemCoreClock/SysTickIHFreq);
	
	while(1) {
	}
}

void ADC_Init(void){
	//Habilitar señal de reloj a GPIOA, GPIOC, ADC12
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN | RCC_AHBENR_ADC12EN;
	// Configurar PA0 como entrada Analogica - Canal 1
	GPIOA->MODER |= (3<<0);
	
	// Configuraer conversiones regulares, resolucion de 8 bits, right align, continuous conversions, overrun mode
	ADC1->CFGR |= (2<<3) | (1<<12) | (1<<13);
	
	// Configurar la secuencia de conversiones, 2 conversiones, una en el canal 7 y otra en el canal 1
	ADC1->SQR1 |= (1<<0) | (1<<6);
	
	// Configurar la frecuencia de reloj del ADC (HCLK/4)
	ADC1_2_COMMON->CCR |= (3<<16);
	
	// Habilitar interrupciones (End of Conversion)
	ADC1->IER |= ADC_IER_EOCIE | ADC_IER_EOSIE; 
	
	// Configurar el ADC para conversiones continuas 
	ADC1->CR |= ADC_CR_ADSTART | ADC_CR_ADEN;

}

void SysTick_Handler(void){
	if(ADC1->ISR & ADC_ISR_EOC)
		adc_data = (uint16_t) ADC1->DR;
	
	float game_bar_pos = (3.3 / 1023) * adc_data;
	
	_n_fps_loop(SysTickIHFreq, (game_bar_pos * 84));
}

void USART2_IRQHandler(void){
	//USART_putString("HOLA");
}

void clk_config(void){
	// PLLMUL <- 0x0E (PLL input clock x 16 --> (8 MHz / 2) * 16 = 64 MHz )  
	RCC->CFGR |= 0xE<<18;
	// Flash Latency, two wait states for 48<HCLK<=72 MHz
	FLASH->ACR |= 0x2;
	// PLLON <- 0x1 
  RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY)) {};	
	// SW<-0x02 (PLL as System Clock), HCLK not divided, PPRE1<-0x4 (APB1 <- HCLK/2), APB2 not divided 
	RCC->CFGR |= 0x402;
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
		
	NVIC_EnableIRQ(USART2_IRQn);
	SystemCoreClockUpdate();	
}

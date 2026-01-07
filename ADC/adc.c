#include "stm32f411xe.h"
#include "adc.h"

#define GPIOAEN		(1<<0)
#define ADC1EN		(1<<8)
#define ADON		(1<<0)
#define	SR_EOC		(1<<1)
#define SWSTART		(1<<30)

void adc1_init(void)
{
	/*
	 * GPIOA Configuration
	 * 		Enable GPIOA clock bus
	 * 		Set PA1 as analog
	 */
	/*
	 * ADC1 Configuration
	 * 		Enable ADC1 clock bus
	 * 		Set Sequence
	 * 		Set Sequence length
	 * 		Enable ADC1
	 */

	// Enable GPIOA clock bus
	RCC->AHB1ENR|=GPIOAEN;

	// Set PA1 as analog
	GPIOA->MODER|=(3<<2);

	// Enable ADC1 clock bus
	RCC->APB2ENR|=ADC1EN;

	// Set Sequence
	ADC1->SQR3=0x01;

	// Set Sequence length
	ADC1->SQR1=0;

	// Enable ADC1
	ADC1->CR2|=ADON;
}

void adc1_start_conversion(void)
{
	ADC1->CR2|=SWSTART;
}

uint16_t adc1_read(void)
{
	while(!(ADC1->SR & SR_EOC));
	return ADC1->DR;
}


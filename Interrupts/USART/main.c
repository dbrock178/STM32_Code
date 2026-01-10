#include "stm32f411xe.h"
#include "usart.h"
#include <stdint.h>

#define GPIOAEN	(1<<0)
#define LED		(1<<5)

uint8_t ch;

int main(void)
{
	RCC->AHB1ENR|=GPIOAEN;
	GPIOA->MODER&=~(1<<11);
	GPIOA->MODER|=(1<<10);
	GPIOA->ODR&=~(1<<5);

	usart1_rx_init(115200);
	for(;;)
	{


	}
}

static void usart_callback(void)
{
	ch=USART1->DR;
	if(ch=='A')
	{
		GPIOA->ODR^=LED;
	}
}

void USART1_IRQHandler(void)
{
	if(USART1->SR & (1<<5))
	{
		usart_callback();
	}
}

#include "stm32f411xe.h"
#include "exti.h"
#include <stdint.h>

#define GPIOAEN	(1<<0)
#define LED		(1<<5)
static void exti_callback(void);

int main(void)
{
	RCC->AHB1ENR|=GPIOAEN;
	GPIOA->MODER&=~(1<<11);
	GPIOA->MODER|=(1<<10);
	GPIOA->ODR&=~(1<<5);

	pc13_exti_init();
	for(;;)
	{


	}
}

static void exti_callback(void)
{
	GPIOA->ODR^=LED;
}

void EXTI15_10_IRQHandler(void)
{
	// PC13 called interrupt
	if(EXTI->PR & LINE13)
	{
		// Clear PR flag to rearm interrupt
		EXTI->PR|=LINE13;

		// Do something
		exti_callback();
	}
}

#include "exti.h"

#define	GPIOCEN		(1<<2)
#define SYSCFGEN	(1<<14)

void pc13_exti_init(void)
{
	/* Disable global interrupts
	 * Enable clock access for GPIOC
	 * Set PC13 as input
	 * Enable clock access for SYSCFG
	 * Select PORTC for EXTI13
	 * Unmask EXTI13
	 * Select falling edge trigger
	 * Enable EXTI line in NVIC
	 * Enable global interrupts
	 */

	// Disable global interrupts
	__disable_irq();

	// Enable clock access for GPIOC
	RCC->AHB1ENR|=GPIOCEN;

	// Set PC13 as input
	GPIOC->MODER&=~(1<<26);
	GPIOC->MODER&=~(1<<27);

	// Enable clock access for SYSCFG
	RCC->APB2ENR|=SYSCFGEN;

	// Select PORTC for EXTI13
	SYSCFG->EXTICR[3]&=~(1<<7);
	SYSCFG->EXTICR[3]&=~(1<<6);
	SYSCFG->EXTICR[3]|=(1<<5);
	SYSCFG->EXTICR[3]&=~(1<<4);

	// Unmask EXTI13
	EXTI->IMR|=(1<<13);

	// Select falling edge trigger
	EXTI->FTSR|=(1<<13);

	// Enable EXTI line in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	// Enable global interrupts
	__enable_irq();
}



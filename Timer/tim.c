#include "tim.h"

#define TIM2EN		(1<<0)
#define CEN 		(1<<0)

void tim2_1hz_init(void)
{
	/*
	 * Enable clock access to tim2
	 * Set prescaler value
	 * Set auto reload value
	 * Clear timer counter
	 * Enable timer
	 */
	RCC->APB1ENR|=TIM2EN;

	// Set prescaler value
	TIM2->PSC = 1600 -1; // 16000000/1600 = 10000
	TIM2->ARR = 10000 - 1; // 10000/10000 = 1
	TIM2->CNT = 0;
	TIM2->CR1|=CEN;
}

#include "systick.h"

#define DELAY_S_LOAD	(16000000) // Default 16 Mhz internal clock
#define DELAY_MS_LOAD	(16000) // Default 16 Mhz internal clock
#define DELAY_US_LOAD	(16) // Default 16 Mhz internal clock
#define	COUNT_FLAG		(1<<16)
#define CLKSOURCE		(1<<2)
#define ENABLE			(1<<0)

void systick_delay_ms(uint16_t delay)
{
	SysTick->LOAD=DELAY_MS_LOAD;
	SysTick->VAL=0;
	SysTick->CTRL= (CLKSOURCE | ENABLE);

	for(uint16_t i=0;i<delay;i++)
	{
		while(!(SysTick->CTRL & COUNT_FLAG));
	}

	SysTick->CTRL=0;
}

void systick_delay_us(uint16_t delay)
{
	SysTick->LOAD=DELAY_US_LOAD;
	SysTick->VAL=0;
	SysTick->CTRL= (CLKSOURCE | ENABLE);

	for(uint16_t i=0;i<delay;i++)
	{
		while(!(SysTick->CTRL & COUNT_FLAG));
	}

	SysTick->CTRL=0;
}

void systick_delay_s(uint16_t delay)
{
	SysTick->LOAD=DELAY_S_LOAD;
	SysTick->VAL=0;
	SysTick->CTRL= (CLKSOURCE | ENABLE);

	for(uint16_t i=0;i<delay;i++)
	{
		while(!(SysTick->CTRL & COUNT_FLAG));
	}

	SysTick->CTRL=0;
}

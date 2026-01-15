#include "fpu.h"

void fpu_enable(void)
{
	// Enable fpu with full access
	SCB->CPACR|=(1<<20);
	SCB->CPACR|=(1<<21);
	SCB->CPACR|=(1<<22);
	SCB->CPACR|=(1<<23);
}

void fpu_disable(void)
{
	SCB->CPACR&=~(1<<20);
	SCB->CPACR&=~(1<<21);
	SCB->CPACR&=~(1<<22);
	SCB->CPACR&=~(1<<23);
}

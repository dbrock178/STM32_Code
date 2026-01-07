#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include "stm32f411xe.h"

void systick_delay_ms(uint16_t delay);
void systick_delay_us(uint16_t delay);
void systick_delay_s(uint16_t delay);

#endif /* SYSTICK_H_ */

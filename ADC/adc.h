#ifndef ADC_H_
#define ADC_H_

#include <stdint.h>

void adc1_init(void);
void adc1_start_conversion(void);
uint16_t adc1_read(void);

#endif /* ADC_H_ */

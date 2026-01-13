#ifndef NEO6_H_
#define NEO6_H_

#include "stm32f411xe.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

#define NEO6_MAX_BUFFER		(100)
#define LATLONG_SIZE		(13)
#define TIME_SIZE			(10)

void neo6_init(void);
uint8_t neo6_get_data(char *time,char *latitude,char *longitude);

#endif /* NEO6_H_ */

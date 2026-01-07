#ifndef USART_H_
#define USART_H_

#include <stdint.h>
#include "stm32f411xe.h"

void usart1_tx_init(uint32_t buad_rate);
void usart1_rx_init(uint32_t buad_rate);
void usart1_write(uint8_t ch);
uint8_t usart1_read(void);

void usart2_tx_init(uint32_t buad_rate);
void usart2_rx_init(uint32_t buad_rate);
void usart2_write(uint8_t ch);
uint8_t usart2_read(void);

void usart6_tx_init(uint32_t buad_rate);
void usart6_rx_init(uint32_t buad_rate);
void usart6_write(uint8_t ch);
uint8_t usart6_read(void);

#endif /* USART_H_ */

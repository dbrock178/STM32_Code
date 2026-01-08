#ifndef MICROCHIP_24LC512_H_
#define MICROCHIP_24LC512_H_

#include "stm32f411xe.h"
#include "i2c_bit_bang.h"
#include <stdint.h>

void micro_eeprom_init(GPIO_TypeDef *gpio,uint8_t clk_pin, uint8_t sda_pin);
uint8_t micro_eeprom_byte_write(uint16_t address,uint8_t data);
uint8_t micro_eeprom_page_write(uint16_t address,uint8_t *data,uint8_t num_of_data);
uint8_t micro_eeprom_current_read(uint8_t *data);
uint8_t micro_eeprom_random_read(uint16_t address,uint8_t *data);
uint8_t micro_eeprom_seq_read(uint16_t address,uint8_t *data,uint16_t num_of_data);

#endif /* MICROCHIP_24LC512_H_ */

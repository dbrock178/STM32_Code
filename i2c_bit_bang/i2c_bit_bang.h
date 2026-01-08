#ifndef I2C_BIT_BANG_H_
#define I2C_BIT_BANG_H_

#include "stm32f411xe.h"
#include "systick.h"
#include <stdint.h>

#define I2C_SUCCESS		1
#define I2C_ERROR		0

void i2c_init(GPIO_TypeDef *gpio,uint8_t scl_pin,uint8_t sda_pin);
uint8_t i2c_write(uint8_t dev_address,uint8_t data);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_get_ack_status(void);
void i2c_write_byte(uint8_t byte);
void i2c_read_byte(uint8_t *byte);
void i2c_send_ack(void);
void i2c_send_nack(void);
void i2c_restart(void);
#endif /* I2C_BIT_BANG_H_ */

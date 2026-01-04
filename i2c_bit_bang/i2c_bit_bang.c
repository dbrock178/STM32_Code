#include "i2c_bit_bang.h"

#define SET_SCL_LOW		(_gpio->BSRR|=(1<<(_scl_pin+16)))
#define SET_SCL_HIGH	(_gpio->BSRR|=(1<<_scl_pin))
#define SET_SDA_LOW		(_gpio->BSRR|=(1<<(_sda_pin+16)))
#define SET_SDA_HIGH	(_gpio->BSRR|=(1<<_sda_pin))

static void delay(uint8_t ms);

static GPIO_TypeDef *_gpio;
static uint8_t _scl_pin;
static uint8_t _sda_pin;

void i2c_init(GPIO_TypeDef *gpio,uint8_t scl_pin,uint8_t sda_pin)
{
	_gpio=gpio;
	_scl_pin=scl_pin;
	_sda_pin=sda_pin;

	// Configure Open Drain
	_gpio->OTYPER|=(1<<_scl_pin);
	_gpio->OTYPER|=(1<<_sda_pin);

	// Configure GPIOx as output
	_gpio->MODER|=(1<<(_scl_pin*2));
	_gpio->MODER&=~(1<<((_scl_pin*2)+1));

	_gpio->MODER|=(1<<(_sda_pin*2));
	_gpio->MODER&=~(1<<((_sda_pin*2)+1));

	// Set Pins High
	SET_SDA_HIGH;
	SET_SCL_HIGH;
}

uint8_t i2c_write(uint8_t dev_address,uint8_t data)
{
	uint8_t ack_status;

	i2c_start();
	write_byte(dev_address<<1); // Shift address to append write bit
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	write_byte(data);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_stop();

	return I2C_SUCCESS;
}

void i2c_start(void)
{
	SET_SDA_LOW;
	delay(1);
	SET_SCL_LOW;
	delay(1);
}

void i2c_stop(void)
{
	SET_SDA_LOW;
	delay(1);
	SET_SCL_HIGH;
	delay(1);
	SET_SDA_HIGH;
	delay(1);
}

uint8_t i2c_get_ack_status(void)
{
	uint8_t ack;

	// Release SDA Line so slave can respond
	SET_SDA_HIGH;
	delay(1);

	// Sample data
	SET_SCL_HIGH;
	delay(1);

	ack=_gpio->IDR&=(1<<_sda_pin);

	SET_SCL_LOW;
	delay(1);

	return ack;
}

void write_byte(uint8_t byte)
{
	uint8_t mask=0x80;
	do
	{
		(mask & byte) ? (SET_SDA_HIGH) : (SET_SDA_LOW);
		delay(1);

		SET_SCL_HIGH;
		delay(1);

		SET_SCL_LOW;
		delay(1);

	}while((mask>>=1));
}

static void delay(uint8_t ms)
{
	for(volatile int i=0;i<ms;i++)
	{
		for(volatile int j=0;j<300;j++);
	}
}

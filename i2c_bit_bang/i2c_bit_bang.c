#include "i2c_bit_bang.h"

#define SET_SCL_LOW		(_gpio->BSRR|=(1<<(_scl_pin+16)))
#define SET_SCL_HIGH	(_gpio->BSRR|=(1<<_scl_pin))
#define SET_SDA_LOW		(_gpio->BSRR|=(1<<(_sda_pin+16)))
#define SET_SDA_HIGH	(_gpio->BSRR|=(1<<_sda_pin))
#define DELAY_US		(50)

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
	i2c_write_byte(dev_address<<1); // Shift address to append write bit
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(data);
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
	systick_delay_us(DELAY_US);

	SET_SCL_LOW;
	systick_delay_us(DELAY_US);
}

void i2c_stop(void)
{
	SET_SDA_LOW;
	systick_delay_us(DELAY_US);

	SET_SCL_HIGH;
	systick_delay_us(DELAY_US);

	SET_SDA_HIGH;
	systick_delay_us(DELAY_US);
}

uint8_t i2c_get_ack_status(void)
{
	uint8_t ack;

	// Release SDA Line so slave can respond
	SET_SDA_HIGH;
	systick_delay_us(DELAY_US);

	// Sample data
	SET_SCL_HIGH;
	systick_delay_us(DELAY_US);

	ack=_gpio->IDR&=(1<<_sda_pin);

	SET_SCL_LOW;
	systick_delay_us(DELAY_US);

	return ack;
}

void i2c_write_byte(uint8_t byte)
{
	uint8_t mask=0x80;
	do
	{
		(mask & byte) ? (SET_SDA_HIGH) : (SET_SDA_LOW);
		systick_delay_us(DELAY_US);

		SET_SCL_HIGH;
		systick_delay_us(DELAY_US);

		SET_SCL_LOW;
		systick_delay_us(DELAY_US);

	}while((mask>>=1));
}

void i2c_read_byte(uint8_t *byte)
{
	SET_SDA_HIGH; // Release so slave can talk
	*byte=0x00;
	for(uint8_t i=0;i<8;i++)
	{
		*byte<<=1;
		SET_SCL_HIGH;
		systick_delay_us(DELAY_US);

		if(_gpio->IDR & (1<<_sda_pin))
		{
			*byte|=0x01;
		}

		SET_SCL_LOW;
		systick_delay_us(DELAY_US);
	}
}

void i2c_send_ack(void)
{
	SET_SDA_LOW;
	systick_delay_us(DELAY_US);

	SET_SCL_HIGH;
	systick_delay_us(DELAY_US);

	SET_SCL_LOW;
	systick_delay_us(DELAY_US);
}

void i2c_send_nack(void)
{
	SET_SDA_HIGH;
	systick_delay_us(DELAY_US);

	SET_SCL_HIGH;
	systick_delay_us(DELAY_US);

	SET_SCL_LOW;
	systick_delay_us(DELAY_US);
}

void i2c_restart(void)
{
	SET_SDA_HIGH;
	systick_delay_us(DELAY_US);

	SET_SCL_HIGH;
	systick_delay_us(DELAY_US);

	i2c_start();
}

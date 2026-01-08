#include "microchip_24LC512.h"

#define DEVICE_ADDRESS		(0x50) // Change if necessary Pins A0-A2=0

static GPIO_TypeDef *_gpio;
static uint8_t _clk_pin,_sda_pin;

// Developer is responsible for setting clock bus access for GPIOx
// beforehand
void micro_eeprom_init(GPIO_TypeDef *gpio,uint8_t clk_pin, uint8_t sda_pin)
{
	_gpio=gpio;
	_clk_pin=clk_pin;
	_sda_pin=sda_pin;

	i2c_init(_gpio, _clk_pin, _sda_pin);
}

uint8_t micro_eeprom_byte_write(uint16_t address,uint8_t data)
{
	uint8_t ack_status;
	uint8_t upper_address=(address>>8);
	uint8_t lower_address= address & 0x00FF;

	i2c_start();
	i2c_write_byte(DEVICE_ADDRESS<<1);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(upper_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(lower_address);
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

uint8_t micro_eeprom_page_write(uint16_t address,uint8_t *data,uint8_t num_of_data)
{
	// If we try to do a page write greater than 128 characters
	// then the eeprom will rollover and overwrite data see
	// datasheet page 10 for details.
	if(num_of_data>128 || num_of_data==0)
	{
		return I2C_ERROR;
	}

	uint8_t ack_status;
	uint8_t upper_address=(address>>8);
	uint8_t lower_address= address & 0x00FF;

	i2c_start();
	i2c_write_byte(DEVICE_ADDRESS<<1);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(upper_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(lower_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	for(volatile int i=0;i<num_of_data;i++)
	{
		i2c_write_byte(*data);
		ack_status=i2c_get_ack_status();

		if(ack_status)
		{
			i2c_stop();
			return I2C_ERROR;
		}

		data++;
	}

	i2c_stop();
	return I2C_SUCCESS;
}

uint8_t micro_eeprom_current_read(uint8_t *data)
{
	// Shift address and set read bit
	uint8_t address=(DEVICE_ADDRESS<<1)+1;
	uint8_t ack_status;

	i2c_start();
	i2c_write_byte(address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_read_byte(data);
	i2c_send_nack();
	i2c_stop();

	return I2C_SUCCESS;
}

uint8_t micro_eeprom_random_read(uint16_t address,uint8_t *data)
{
	uint8_t ack_status;
	uint8_t upper_address=(address>>8);
	uint8_t lower_address= address & 0x00FF;

	i2c_start();
	i2c_write_byte(DEVICE_ADDRESS<<1);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(upper_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(lower_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_restart();

	i2c_write_byte((DEVICE_ADDRESS<<1)+1);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_read_byte(data);
	i2c_send_nack();
	i2c_stop();

	return I2C_SUCCESS;
}

uint8_t micro_eeprom_seq_read(uint16_t address,uint8_t *data,uint16_t num_of_data)
{
	if(num_of_data<2)
	{
		return I2C_ERROR;
	}

	uint8_t ack_status;
	uint8_t upper_address=(address>>8);
	uint8_t lower_address= address & 0x00FF;

	i2c_start();
	i2c_write_byte(DEVICE_ADDRESS<<1);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(upper_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_write_byte(lower_address);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	i2c_restart();

	i2c_write_byte((DEVICE_ADDRESS<<1)+1);
	ack_status=i2c_get_ack_status();

	if(ack_status)
	{
		i2c_stop();
		return I2C_ERROR;
	}

	for(volatile uint16_t i=0;i<num_of_data;i++)
	{
		i2c_read_byte(data);
		data++;
		if(i==(num_of_data-1))
		{
			i2c_send_nack(); // Last byte read sends NACK
		}
		else
		{
			i2c_send_ack(); // Send ACK unless last byte
		}
	}

	i2c_stop();

	return I2C_SUCCESS;
}

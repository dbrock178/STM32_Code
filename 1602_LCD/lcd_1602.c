#include <stdint.h>
#include "i2c_bit_bang.h"
#include "lcd_1602.h"

uint8_t lcd_append_display_control_command(void);

// Sets hardware pin states
static uint8_t _backlight,_rw,_rs;

/* Different configurations for LCD display
   see datasheet pages 24 & 25
*/
static uint8_t _lines,_d,_c,_b;

// See page 46 of datasheet for procedure
void lcd_init(uint8_t lines)
{
	_lines=lines;
	lcd_write_command(0x33);
	lcd_write_command(0x32);
	(_lines) ? lcd_write_command(0x28) : lcd_write_command(0x20);//0x20
	lcd_write_command(LCD_ENTRY_MODE_SET);
	lcd_write_command(LCD_CLEAR_DISPLAY);
	//lcd_write_command(0x06);
	lcd_set_entry_mode(LCD_CURSOR_MOVE_RIGHT, LCD_ENTRY_MODE_NO_SHIFT);
}

void lcd_display_off(void)
{
	_d=LCD_DISPLAY_OFF;
	uint8_t command=lcd_append_display_control_command();
	lcd_write_command(command);
}

void lcd_display_on(void)
{
	_d=LCD_DISPLAY_ON;
	uint8_t command=lcd_append_display_control_command();
	lcd_write_command(command);
}

void lcd_backlight_on(void)
{
	_backlight=LCD_BACKLIGHT_ON;
	i2c_write(LCD_ADDRESS, (1<<LCD_BL_PIN));
}

void lcd_backlight_off(void)
{
	_backlight=LCD_BACKLIGHT_OFF;
	i2c_write(LCD_ADDRESS,0x00);
}

void lcd_blink_on(void)
{
	_b=LCD_BLINK_ON;
	uint8_t command=lcd_append_display_control_command();
	lcd_write_command(command);
}

void lcd_blink_off(void)
{
	_b=LCD_BLINK_OFF;
	uint8_t command=lcd_append_display_control_command();
	lcd_write_command(command);
}

void lcd_cursor_on(void)
{
	_c=LCD_CURSOR_ON;
	uint8_t command=lcd_append_display_control_command();
	lcd_write_command(command);
}

void lcd_cursor_off(void)
{
	_c=LCD_CURSOR_OFF;
	uint8_t command=lcd_append_display_control_command();
	lcd_write_command(command);
}

void lcd_screen_shift_left(void)
{
	lcd_write_command(LCD_SCREEN_SHIFT_LEFT);
}

void lcd_screen_shift_right(void)
{
	lcd_write_command(LCD_SCREEN_SHIFT_RIGHT);
}

void lcd_cursor_shift_left(void)
{
	lcd_write_command(LCD_CURSOR_SHIFT_LEFT);
}

void lcd_cursor_shift_right(void)
{
	lcd_write_command(LCD_CURSOR_SHIFT_RIGHT);
}

void lcd_clear(void)
{
	lcd_write_command(LCD_CLEAR_DISPLAY);
	for(uint16_t i=0;i<50000;i++);
}

void lcd_return_home(void)
{
	lcd_write_command(LCD_RETURN_HOME);
}

void lcd_set_entry_mode(uint8_t cursor_dir,uint8_t display_shift)
{
	uint8_t command=LCD_ENTRY_MODE_SET;

	if(cursor_dir)
	{
		command|=(1<<1);
	}

	if(display_shift)
	{
		command|=(1<<0);
	}

	lcd_write_command(command);
}

void lcd_set_ddram_address(uint8_t address)
{
	uint8_t command=LCD_SET_DDRAM;
	command|=address;
	lcd_write_command(command);
}

void lcd_set_cgram_address(uint8_t address)
{
	uint8_t command=LCD_SET_CGRAM;
	command|=address;
	lcd_write_command(command);
}

void lcd_write_command(uint8_t command)
{
	_rw=0;
	_rs=0;
	lcd_write_byte(command);
}

void lcd_write_data(uint8_t data)
{
	_rs=1;
	_rw=0;
	lcd_write_byte(data);
}

void lcd_write_byte(uint8_t byte)
{
	uint8_t upper_nibble=(byte & 0xF0);
	uint8_t lower_nibble=(byte<<4);

	if(_backlight)
	{
		upper_nibble|=(1<<LCD_BL_PIN);
		lower_nibble|=(1<<LCD_BL_PIN);
	}

	if(_rw)
	{
		upper_nibble|=(1<<LCD_RW_PIN);
		lower_nibble|=(1<<LCD_RW_PIN);
	}

	if(_rs)
	{
		upper_nibble|=(1<<LCD_RS_PIN);
		lower_nibble|=(1<<LCD_RS_PIN);
	}

	upper_nibble|=(1<<LCD_E_PIN);
	i2c_write(LCD_ADDRESS, upper_nibble); //Send data
	upper_nibble&=~(1<<LCD_E_PIN);
	i2c_write(LCD_ADDRESS, upper_nibble);

	lower_nibble|=(1<<LCD_E_PIN);
	i2c_write(LCD_ADDRESS, lower_nibble); //Send data
	lower_nibble&=~(1<<LCD_E_PIN);
	i2c_write(LCD_ADDRESS, lower_nibble);
}

uint8_t lcd_append_display_control_command(void)
{
	uint8_t command=LCD_DISPLAY_CONTROL;
	if(_d)
	{
		command|=(1<<2);
	}

	if(_c)
	{
		command|=(1<<1);
	}

	if(_b)
	{
		command|=(1<<0);
	}

	return command;
}

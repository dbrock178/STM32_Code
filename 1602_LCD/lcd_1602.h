#ifndef LCD_1602_H_
#define LCD_1602_H_

#include <stdint.h>

#define LCD_ADDRESS				(0x27) //Change if necessary

#define LCD_CLEAR_DISPLAY		(0x01)
#define LCD_RETURN_HOME			(0x02)
#define LCD_ENTRY_MODE_SET		(0x04)
#define LCD_DISPLAY_CONTROL		(0x08)
#define LCD_CURSOR_DISPLAY		(0x10)
#define LCD_FUNCTION_SET		(0x20)
#define LCD_SET_CGRAM			(0x40)
#define LCD_SET_DDRAM			(0x80)

#define LCD_RS_PIN				(0x00)
#define LCD_RW_PIN				(0x01)
#define LCD_E_PIN				(0x02)
#define LCD_BL_PIN				(0x03)

#define LCD_DISPLAY_OFF			(0x00)
#define LCD_DISPLAY_ON			(0x01)
#define LCD_BACKLIGHT_OFF		(0x00)
#define LCD_BACKLIGHT_ON		(0x01)
#define LCD_BLINK_OFF			(0x00)
#define LCD_BLINK_ON			(0x01)
#define LCD_CURSOR_OFF			(0x00)
#define LCD_CURSOR_ON			(0x01)
#define LCD_SCREEN_SHIFT_LEFT 	(0x18)
#define LCD_SCREEN_SHIFT_RIGHT	(0x1C)
#define LCD_CURSOR_SHIFT_LEFT	(0x10)
#define LCD_CURSOR_SHIFT_RIGHT	(0x14)
#define LCD_CURSOR_MOVE_LEFT	(0x00)
#define LCD_CURSOR_MOVE_RIGHT	(0x01)
#define LCD_ENTRY_MODE_SHIFT	(0x01)
#define LCD_ENTRY_MODE_NO_SHIFT	(0x00)

void lcd_init(uint8_t lines);
void lcd_display_off(void);
void lcd_display_on(void);
void lcd_backlight_on(void);
void lcd_backlight_off(void);
void lcd_blink_on(void);
void lcd_blink_off(void);
void lcd_cursor_on(void);
void lcd_cursor_off(void);
void lcd_screen_shift_left(void);
void lcd_screen_shift_right(void);
void lcd_cursor_shift_left(void);
void lcd_cursor_shift_right(void);
void lcd_clear(void);
void lcd_return_home(void);
void lcd_set_entry_mode(uint8_t cursor_dir,uint8_t display_shift);
void lcd_set_ddram_address(uint8_t address);
void lcd_set_cgram_address(uint8_t address);
void lcd_write_command(uint8_t command);
void lcd_write_data(uint8_t data);
void lcd_write_byte(uint8_t data);

#endif /* LCD_1602_H_ */

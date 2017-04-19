/*
 * lcd.h
 *
 * Created: 2017-04-18 13:21:52
 *  Author: 4u6u5
 */ 


#ifndef _HA_LCD_H_
#define _HA_LCD_H_

#define DISP_CONF_N 0 // Line number (0: 1 line display, 1: 2 line display)
#define DISP_CONF_F 0 // Font type (0: 5x8 dots, 1: 5x11 dots)

#define DISP_CONF_D 1 // Display ON/OFF (when off, data still stored in DDRAM)
#define DISP_CONF_C 1 // Cursor ON/OFF
#define DISP_CONF_B 0 // Cursor blink ON/OFF

#define DISP_CONF_I_D 1 // Cursor increment/decrement (1: increment)
#define DISP_CONF_S 0 // Shift (0: no shift)

void lcd_init(void);
void lcd_pin_write(void);
void lcd_pin_cmd(int RS, int B7, int B6, int B5, int B4);
void lcd_write_char(char c);

#endif /* _HA_LCD_H_ */
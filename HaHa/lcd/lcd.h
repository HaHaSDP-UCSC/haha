/**
 * @file lcd.h
 * @brief LCD display general code
 * @author August Valera (avalera)
 * @version
 * @date 2017-04-20
 */


#ifndef _HA_LCD_H_
#define _HA_LCD_H_

#include "lcddevice.h"
#include "utils/hahaUtils.h"

char lcd_buffer[LCD_ROWS][LCD_COLS + 1]; // Extra space for null terminator

void lcd_init();
void lcd_update();
void lcd_clear();
void lcd_set_line(int row, char* str);
void lcd_set_line_overflow(int row, char* str);
void lcd_set_char(int row, int col, char c);

#endif /* _HA_LCD_H_ */

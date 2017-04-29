/**
 * @file lcd.h
 * @brief LCD display general code
 * @author August Valera (avalera)
 * @version
 * @date 2017-04-20
 */

#include "lcd.h"

void lcd_init() {
	_lcd_init();
}

void lcd_update() {
	_lcd_update(lcd_buffer);
}

void lcd_clear() {
	for(int row = 0; row < LCD_ROWS; row++)
		for(int col = 0; col < LCD_COLS; col++)
			lcd_buffer[row][col] = " ";
}

void lcd_set_line(int row, char* str) {
	if(row >= 0 && row < LCD_ROWS) {
		strncpy(lcd_buffer[row][0], str, LCD_COLS);
		for(int col = strlen(str); col < LCD_COLS; col++)
			lcd_buffer[row][col] = " ";
	}
}

void lcd_set_char(int row, int col, char c) {
	if(row >= 0 && row < LCD_ROWS)
		if(col >= 0 && col < LCD_COLS)
			lcd_buffer[row][col] = c;
}

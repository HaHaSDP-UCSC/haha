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
	lcd_clear();
}

void lcd_update() {
	_lcd_update(lcd_buffer);
}

void lcd_clear() {
	for(int row = 0; row < LCD_ROWS; row++) {
		for(int col = 0; col < LCD_COLS; col++)
			lcd_buffer[row][col] = ' ';
		lcd_buffer[row][LCD_COLS] = '\0';
	}
}

void lcd_set_line(int row, char* str) {
	if(row >= 0 && row < LCD_ROWS) {
		strncpy(lcd_buffer[row], str, LCD_COLS);
		int col = strlen(str);
		for(; col < LCD_COLS; col++)
			lcd_buffer[row][col] = ' ';
	}
}

void lcd_set_line_overflow(int row, char* str) {
	if(row < LCD_ROWS) {
		lcd_set_line(row, str);
		lcd_set_line_overflow(row + 1, str + LCD_COLS);
	}
}

void lcd_set_char(int row, int col, char c) {
	if(row >= 0 && row < LCD_ROWS)
		if(col >= 0 && col < LCD_COLS)
			lcd_buffer[row][col] = c;
}

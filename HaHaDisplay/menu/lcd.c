/*
 * lcd.c
 *
 * Created: 2017-04-18 12:56:40
 *  Author: August Valera (avalera)
 */ 

#include "atmel_start.h"
#include "utils/hahaUtils.h"
#include "lcd.h"

void lcd_init(void) {
	gpio_set_pin_level(DISP_E, false);
	// Function set
	lcd_pin_cmd(0, 0, 0, 1, 1);
	delay(100);
	// Function set
	lcd_pin_cmd(0, 0, 0, 1, 0);
	lcd_pin_cmd(0, DISP_CONF_N, DISP_CONF_F, -1, -1);
	delay(100);
	// Function set
	lcd_pin_cmd(0, 0, 0, 1, 0);
	lcd_pin_cmd(0, DISP_CONF_N, DISP_CONF_F, -1, -1);
	delay(100);
	// Display ON/OFF control
	lcd_pin_cmd(0, 0, 0, 0, 0);
	lcd_pin_cmd(0, 1, DISP_CONF_D, DISP_CONF_C, DISP_CONF_B);
	delay(100);
	// Display clear
	lcd_pin_cmd(0, 0, 0, 0, 0);
	lcd_pin_cmd(0, 0, 0, 0, 1);
	delay(100);
	// Entry mode set
	lcd_pin_cmd(0, 0, 0, 0, 0);
	lcd_pin_cmd(0, 0, 1, DISP_CONF_I_D, DISP_CONF_S);
	delay(100);
}

void lcd_pin_write(void) {
	gpio_set_pin_level(DISP_E, true);
	delay(10);
	gpio_set_pin_level(DISP_E, false);
}

void lcd_pin_cmd(int RS, int B7, int B6, int B5, int B4) {
	if(RS >= 0) gpio_set_pin_level(DISP_CTRST, RS ? true : false);
	if(B7 >= 0) gpio_set_pin_level(DISP_B3, B7 ? true : false);
	if(B6 >= 0) gpio_set_pin_level(DISP_B2, B6 ? true : false);
	if(B5 >= 0) gpio_set_pin_level(DISP_B1, B5 ? true : false);
	if(B4 >= 0) gpio_set_pin_level(DISP_B0, B4 ? true : false);
	lcd_pin_write();
}

void lcd_write_char(char c) {
	lcd_pin_cmd(1, c & (1 << 3), c & (1 << 2), c & (1 << 1), c & (1 << 0));
}
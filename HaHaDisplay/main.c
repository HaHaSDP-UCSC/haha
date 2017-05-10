#include <atmel_start.h>
#include "lcd/lcd.h"
#include "menu/ui.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	
	lcd_clear();
	
	/*
	int i = 0, j = 0;
	while (1) {
		if(i == 0 && j == 0) lcd_clear();
		char c = j < 10 ? '0' + j : 'a' + j - 10;
		lcd_set_char(i, j, c);
		if((j = ++j % LCD_COLS) == 0)
			i = ++i % LCD_ROWS;
		lcd_update();
		delay(300);
	}
	*/
	Menu* menu = ui_init();
	menu_move(menu, MENU_DOWN);
	menu_move(menu, MENU_UP);
	menu_set_lcd(menu);
	while(1) {
		if(! gpio_get_pin_level(BTN_DOWN)) {
			menu_move(menu, MENU_DOWN);
			menu_set_lcd(menu);
		} else if(! gpio_get_pin_level(BTN_RIGHT_TEMP)) {
			menu_move(menu, MENU_RIGHT);
			menu_set_lcd(menu);
		} else if(! gpio_get_pin_level(BTN_UP)) {
			menu_move(menu, MENU_UP);
			menu_set_lcd(menu);
		} else if(! gpio_get_pin_level(BTN_LEFT_TEMP)) {
			menu_move(menu, MENU_LEFT);
			menu_set_lcd(menu);
		}
		delay(100);
	}
}

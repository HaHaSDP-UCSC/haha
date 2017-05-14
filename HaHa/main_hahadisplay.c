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
	
	// Start screen
	lcd_set_line(0, "JACK BASKIN ENGR");
	lcd_set_line(1, "Home  Assistance");
	lcd_set_line(2, "   Help Alert");
	lcd_set_line(3, " Button Project");
	lcd_update();
	
	Menu* menu = ui_init();
	while(1) {
		if(! gpio_get_pin_level(BTN_DOWN)) {
			menu_move(menu, MENU_DOWN);
		} else if(! gpio_get_pin_level(BTN_RIGHT_TEMP)) {
			menu_move(menu, MENU_RIGHT);
		} else if(! gpio_get_pin_level(BTN_UP)) {
			menu_move(menu, MENU_UP);
		} else if(! gpio_get_pin_level(BTN_LEFT_TEMP)) {
			menu_move(menu, MENU_LEFT);
		}
		delay(100);
	}
}

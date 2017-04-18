#include <atmel_start.h>
#include "menu/lcd.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	while (1) {
	}
}

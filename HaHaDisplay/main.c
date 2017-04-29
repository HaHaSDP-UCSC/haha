#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	
	bool hello = true;
	while (1) {
		_lcd_clear();
		if(hello) {
			_lcd_write_string("Hello World!");
			_lcd_line_next();
			_lcd_write_string("This is a test.");
		} else {
			_lcd_write_string("HA-HA Button SDP Project @UCSC");
		}
		delay(5000);
		hello = !hello;
	}
}

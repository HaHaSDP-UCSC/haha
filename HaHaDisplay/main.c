#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	
	bool hello = true;
	char* string;
	while (1) {
		if(hello) string = "Hello, world!";
		else string = "HA-HA Button SDP Project @UCSC";
		_lcd_write_string(string);
		if(hello) {
			_lcd_line_next();
			_lcd_write_string("This is a test.");
		}
		delay(5000);
		_lcd_clear();
		hello = !hello;
	}
}

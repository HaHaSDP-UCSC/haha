#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	_lcd_init();
	
	bool hello = true;
	char* string;
	while (1) {
		if(hello) string = "Hello, world!";
		else string = "HA-HA Button SDP Project @UCSC";
		for(int i = 0; i < strlen(string); i++) {
			_lcd_write_char(string[i]);
		}
		delay(5000);
		_lcd_clear();
		hello = !hello;
	}
}

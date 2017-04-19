#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	char* hello = "Hello world!";
	/*for(int i = 0; i < strlen(hello); i++) {
		lcd_write_char(hello[i]);
	}
	*/
	lcd_write_char('h');
	while (1) {
	}
}

#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	lcd_clrscr();
	lcd_puts("Hello World...");
	
	while (1) {
	}
}

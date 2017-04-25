#include <atmel_start.h>

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	lcd_init();
	/*
	for(int i = 0; i < 10; i++) {
		delay(1000);
		printf("%d\n", i);
	}
	*/
	char* hello = "Hello world!";
	for(int i = 0; i < strlen(hello); i++) {
		lcd_write_char(hello[i]);
	}
	
	while (1) {
		
	}
}

#include <atmel_start.h>
#include "stdio_start.h"
#include "devstuff/uart.h"
#include <string.h>
#include "xbee/xbee.h"
#include "devstuff/hahaUtils.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	stdio_redirect_init();
	uart_init_irqs();
	xbee_init();
	printf("Starting System...\n");
	xbee_setAPI(1);
	xbee_send("0013a200414F50E5", "This is a test of the service here");
	//uart_write(7E0005085241500000,18 );afj;ld
	SET_SEND_XBEE(TRUE);
	while (1) {
		uart_read();
	}
}


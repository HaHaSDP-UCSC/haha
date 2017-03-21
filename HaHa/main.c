#include <atmel_start.h>
#include "stdio_start.h"
#include "uart/uart.h"
#include <string.h>
#include "networkdevice/networkdevice.h"
#include "utils/hahaUtils.h"


void printpacket1(char* d, uint8_t len){
    printf("in Callback print\n");
    printBuff(d, len, "%c");
    printf("end cb print\n");
}

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
    stdio_redirect_init();
    uart_init_irqs();
    xbee_init();
    printf("Starting System...\n");
    xbee_setAPI(1);
    xbee_send("0013a200414F50E5", "Test Data", 9);
    delay(1000); //wait for xBee response
    //uart_write(7E0005085241500000,18 );afj;ld
    SET_SEND_XBEE(TRUE);
    xbee_register_callback(printpacket1);
    while (1) {
        uart_read();
    }
}


/*
 * @file main.c
 * @brief Main program of the HaHa Alert System.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 
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
	printe("Test STDERR Print\n");
    xbee_setAPI(1);
    xbee_send("0013a200414F50EA", "Test Data", 9);
    //delay(1000); //wait for xBee response
    //uart_write(7E0005085241500000,18 );
    uart_register_netdev_callback(xbee_recv);
    SET_SEND_NETDEV(TRUE);
    xbee_register_callback(printpacket1, FRAME_RX);
    while (1) {
        uart_read();
    }
}


/*
 * @file main.c
 * @brief Main program of the HaHa Alert System.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 
#include <atmel_start.h>
#include "stdio_start.h"
#include "uart/uart.h"
//#include <string.h>
#include "networkdevice/networkdevice.h"
#include "network/network.h"
#include "utils/hahaUtils.h"

void printpacket1(char* d, uint8_t len){
    printf("in Callback print\n");
    printBuff(d, len, "%c");
    printf("end cb print\n");
}

void init_sys(){
    printf("Starting System...\n");
    atmel_start_init();
    stdio_redirect_init();
    uart_init_irqs();
    network_init();
    xbee_setAPI(1);    
}

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    init_sys();
    xbee_send(0x0013a200414F50EA, "Test Data", 9);
    uart_register_netdev_callback(xbee_recv);
    SET_SEND_NETDEV(true);
    xbee_register_callback(app_packet_handler, FRAME_RX);
    
    /* Main Application Loop */
    while (1) {
        uart_read();
    }
}


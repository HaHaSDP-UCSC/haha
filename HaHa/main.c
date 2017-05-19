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
//#include "network/network.h"
#include "utils/hahaUtils.h"
#include "messagequeue/messagequeue.h"
#include "neighbor/friendlist.h"

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
    initMessageQueue();
    //xbee_setAPI(1);    
}

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
	init_sys();
	
	lcd_init();
	lcd_clear();
	ui_init();
	
	// Start screen
	lcd_set_line(0, "JACK BASKIN ENGR");
	lcd_set_line(1, "Home  Assistance");
	lcd_set_line(2, "   Help Alert");
	lcd_set_line(3, " Button Project");
	lcd_update();
	
	while(1) {
		 delay(100);
		 ui_update();
	}
	
    //xbee_send(0x0013a200414F50EA, "Test Data", 9);
    uart_register_netdev_callback(xbee_recv);
    SET_SEND_NETDEV(true);
    xbee_register_callback(app_packet_handler, FRAME_RX);
    //delay(5000);
    //cmd_AT_get("SH");
    //delay(500);
    //cmd_AT_get("NH");
    /* Main Application Loop */
    char buff[80]; // large enough
    int count = 0;
    char c;
    
    addTestFriend("Brian", "Nichols","0013A200414F50EA");
    addTestLocalUser("Kevin", "Lee", 0x1);
    send_ping_request(&friendList[1]);
    
    send_help_request(friendList[1]);
	
    while (1) {
        uart_read();              
        //ui_update();
    }
}


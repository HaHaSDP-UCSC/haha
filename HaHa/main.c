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
#include "ble_base_station.h"

void printpacket1(char* d, uint8_t len){
    printf("in Callback print\n");
    printBuff(d, len, "%c");
    printf("end cb print\n");
}

void init_sys(){
    printf("Starting System...\n");
    
    network_init();
    initMessageQueue();
    xbee_setAPI(1);    
}

void test(){
    printf("hey!");
}    

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    //init_sys();
	atmel_start_init();
    stdio_redirect_init();
    uart_init_irqs();
    //xbee_send(0x0013a200414F50EA, "Test Data", 9);
    //xbee_send(0x0013a200414F50EA, "Test Data", 9);
    //uart_register_netdev_callback(xbee_recv);
   // SET_SEND_NETDEV(true);
    //xbee_register_callback(app_packet_handler, FRAME_RX);
    main1();
    //delay(5000);
    //delay(500);
    
    /* Main Application Loop */
    char buff[80]; // large enough
    int count = 0;
    char c;
    
    //addTestFriend("Brian", "Nichols","0013A200414F50EA");
    //addTestLocalUser("Kevin", "Lee", 0x1);
    //send_ping_request(&friendList[1]);
    
    //send_help_request(friendList[1]);
    addTestFriend("Brian", "Nichols","0013A200414F50EA");
    addTestLocalUser("Kevin", "Lee", 0x1);
    send_ping_request(&friendList[1]);
	
	/* Testing Application code */
	/**
	LocalUser self;
	strcpy(self.friend.firstname, "Kevin");
	strcpy(self.friend.lastname, "Lee");
	strcpy(self.friend.networkaddr, "DEST001");
	self.friend.port = 0x0001;
	
	Friend f;
	strcpy(f.firstname, "Brian");
	strcpy(f.lastname, "Nichols");
	strcpy(f.networkaddr, "DEST002");
	f.port = 0x0002;
	
	send_help_request(&f, &self);
	send_help_request_ack(Friend *f, LocalUser *self);
	
	bool accept = true;
    send_help_response(&f, &self, accept);
	//next accept should come from the help response.
    send_help_response_ack(&f, &self, accept);
	*/
	
	lcd_init();
    lcd_clear();
    ui_init();

    // Start screen
    lcd_set_line(0, "JACK BASKIN ENGR");
    lcd_set_line(1, "Home  Assistance");
    lcd_set_line(2, "   Help Alert");
    lcd_set_line(3, " Button Project");
    lcd_update();
	
    send_help_request(friendList[1]);
    while (1) {
        uart_read();
		ui_update();
		// delay(100);
    }

   while (1);
   return 1;
}

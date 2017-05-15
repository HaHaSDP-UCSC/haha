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
    atmel_start_init();
    stdio_redirect_init();
    uart_init_irqs();
    network_init();
    initMessageQueue();
    //xbee_setAPI(1);    
}

int test(){
    printf("hey!");
}    
int main(void)
{
    /* Initializes MCU, drivers and middleware */
    //init_sys();
    //xbee_send(0x0013a200414F50EA, "Test Data", 9);
   // uart_register_netdev_callback(xbee_recv);
    //SET_SEND_NETDEV(true);
    //xbee_register_callback(app_packet_handler, FRAME_RX);
    main1();
    //delay(5000);
    //cmd_AT_get("SH");
    //delay(500);
    //cmd_AT_get("NH");
    /* Main Application Loop */
    //char buff[80]; // large enough
    //int count = 0;
    //char c;
    
    //addTestFriend("Brian", "Nichols","0013A200414F50EA");
    //addTestLocalUser("Kevin", "Lee", 0x1);
    //send_ping_request(&friendList[1]);
    
    //send_help_request(friendList[1]);
    //while (1) {
    //    uart_read();              
        
   // }
}

/* Main and Atmel pins of HaHaDisplay
#include <atmel_start.h>
#include "lcd/lcd.h"
#include "menu/ui.h"

int main(void)
{
	// Initializes MCU, drivers and middleware
	atmel_start_init();

	// Replace with your application code 
	lcd_init();
	lcd_clear();
	
	// Start screen
	lcd_set_line(0, "JACK BASKIN ENGR");
	lcd_set_line(1, "Home  Assistance");
	lcd_set_line(2, "   Help Alert");
	lcd_set_line(3, " Button Project");
	lcd_update();
	
	Menu* menu = ui_init();
	while(1) {
		if(! gpio_get_pin_level(BTN_DOWN)) {
			menu_move(menu, MENU_DOWN);
		} else if(! gpio_get_pin_level(BTN_RIGHT_TEMP)) {
			menu_move(menu, MENU_RIGHT);
		} else if(! gpio_get_pin_level(BTN_UP)) {
			menu_move(menu, MENU_UP);
		} else if(! gpio_get_pin_level(BTN_LEFT_TEMP)) {
			menu_move(menu, MENU_LEFT);
		}
		delay(100);
	}
}

#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

#define BTN_LEFT GPIO(GPIO_PORTA, 3)
#define BTN_UP GPIO(GPIO_PORTA, 4)
#define BTN_LEFT_TEMP GPIO(GPIO_PORTA, 11)
#define BTN_RIGHT_TEMP GPIO(GPIO_PORTA, 12)
#define DISP_BRIGHT GPIO(GPIO_PORTA, 13)
#define DISP_B3 GPIO(GPIO_PORTA, 14)
#define DISP_B2 GPIO(GPIO_PORTA, 15)
#define DISP_B1 GPIO(GPIO_PORTA, 16)
#define DISP_B0 GPIO(GPIO_PORTA, 17)
#define DISP_E GPIO(GPIO_PORTA, 18)
#define DISP_REG GPIO(GPIO_PORTA, 19)
#define DISP_CTRST GPIO(GPIO_PORTA, 20)
#define BTN_RIGHT GPIO(GPIO_PORTA, 22)
#define BTN_DOWN GPIO(GPIO_PORTA, 23)

#endif // ATMEL_START_PINS_H_INCLUDED
*/

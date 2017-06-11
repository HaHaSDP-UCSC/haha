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
#include "lcd/lcd.h"
#include "menu/ui.h"
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
	xbee_setAPI(1);
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	init_sys();
	toglight = 0;
	int lighton = 0;
	
	//Set PWM
	//pwm_sync_disable(&PWM_0);
	//pwm_sync_set_parameters(&PWM_0, 4, 50);
	//pwm_sync_enable(&PWM_0);
	
	//Set Light Toggle
	gpio_set_pin_level(TOGGLE_LIGHT, false);
	//
	//
	lcd_init();
	lcd_clear();
	ui_init();
	uart_register_netdev_callback(netdevice_recv);
	SET_SEND_NETDEV(true);
	netdevice_register_callback(app_packet_handler, FRAME_RX);
	//
	// Start screen
	lcd_set_line(0, "UCSC/BASKIN ENGR");
	lcd_set_line(1, "   Help Alert");
	lcd_set_line(2, "Home  Assistance");
	lcd_set_line(3, " Button Project");
	lcd_update();
	//max 500 hz for hearing aids
	int count1 = 0;
	//addTestUsers();
	//send_friend_request(&friendList[0], &localUsers[0]);
	while(1) {
		//delay(100);
		count1++;
		uart_read();
		if(count1 % 5000*100 == 0 ){
			ui_update();
			if(toglight){
				if(lighton++ % 100 == 0)
					toglight = false;
				gpio_toggle_pin_level(TOGGLE_LIGHT);
			}
		}
	}
	
	return 1;
}


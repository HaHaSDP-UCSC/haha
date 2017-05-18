/**
* \file
*
* \brief BLE Observer application
*
* Copyright (c) 2016 Atmel Corporation. All rights reserved.
*
* \asf_license_start
*
* \page License
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
*    this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*
* 3. The name of Atmel may not be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an
*    Atmel micro controller product.
*
* THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* \asf_license_stop
*
*/

/*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel
*Support</a>
*/

/**
* \mainpage
* \section preface Preface
* This is the reference manual for the Observer Application
*/
/*- Includes ---------------------------------------------------------------*/

#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "ble_config.h"
#include "app_bsp.h"
#include "at_ble_api.h"
#include "ble_base_station.h"
#include "platform.h"
#include "ble_utils.h"
#include "ble_base_manager.h"
#include "atmel_start_pins.h"
#include "hal_ext_irq.h"
#include "driver_examples.h"
#include "stdio_io.h"
#include "bsscp.h"
#include "hpl_usart_async.h"

//#include "sleep_manager_main.h"

at_ble_addr_t BlueNRG1 = {AT_BLE_ADDRESS_PUBLIC,{0xaa, 0x00, 0x00, 0xE1, 0x80, 0x02}};

#define CONSOLE_IO SAMB11_IO
extern uint8_t scan_response_count;

volatile bool button_pressed = false;

extern bool write_flag;
/* Buffer data to be send over the air */
uint8_t send_data[APP_TX_BUF_SIZE];

/* Data length to be send over the air */
uint16_t send_length = 0;

struct _usart_async_device SAMB11_IO;

#define USART_BUF_SIZE   16
#define UART_RX_COMPLETE 2
#define NUM_BYTES 1

/*Function Definitions */

//static void button_cb(void);
//static void app_timer1_cb(void);
//static void app_timer2_cb(void);

void LED_init(void)
{
	//gpio_set_pin_direction(BLE_APP_LED, GPIO_DIRECTION_OUT);
	//gpio_set_pin_pull_mode(BLE_APP_LED, GPIO_PULL_OFF);
	//gpio_set_pin_function(BLE_APP_LED, GPIO_PIN_FUNCTION_OFF);
	//gpio_set_pin_level(BLE_APP_LED, true);
}

void LED_On(void)
{
	//gpio_set_pin_level(BLE_APP_LED, false);
}

void LED_Off(void)
{
	//gpio_set_pin_level(BLE_APP_LED, true);
}

void LED_Toggle(void)
{
	//gpio_toggle_pin_level(BLE_APP_LED);
}
/*
static void button_cb(void)
{
	DBG_LOG("button pressed");
	button_pressed = true;
}
*/
/*
static void app_timer1_cb(void)
{
// Application may use this timer & callback
}

static void app_timer2_cb(void)
{
// Application may use this timer & callback
}
*/

/*
static void uart_rx_callback(uint8_t input)
{
	if(input == '\r') {
		if(send_length) {
			send_plf_int_msg_ind(UART_RX_COMPLETE, UART_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK, send_data, send_length);
			memset(send_data, 0, APP_TX_BUF_SIZE);
			send_length = 0;
			DBG_LOG(" ");

		}
		} else {
		send_data[send_length++] = input;
		DBG_LOG_CONT("%c", input);

		if(send_length >= APP_TX_BUF_SIZE) {
			send_plf_int_msg_ind(UART_RX_COMPLETE, UART_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK, send_data, send_length);
			memset(send_data, 0, APP_TX_BUF_SIZE);
			send_length = 0;
		}
	}
}
*/

/* Function used for send data */
static void csc_app_send_buf(void)
{
	uint16_t plf_event_type;
	uint16_t plf_event_data_len;
	uint8_t plf_event_data[APP_TX_BUF_SIZE] = {0, };

	platform_event_get(&plf_event_type, plf_event_data, &plf_event_data_len);

	if(plf_event_type == ((UART_RX_INTERRUPT_MASK_RX_FIFO_NOT_EMPTY_MASK << 8) | UART_RX_COMPLETE)) {
		csc_prf_send_data(plf_event_data, plf_event_data_len);
	}

}


/* Function used for receive data */
static void csc_app_recv_buf(uint8_t *recv_data, uint8_t recv_len)
{
	uint16_t ind = 0;
	if (recv_len) {
		for (ind = 0; ind < recv_len; ind++) {
			DBG_LOG_CONT("%c", recv_data[ind]);
		}
		DBG_LOG("\r\n");
	}
}

/* Callback called for new data from remote device */
static void csc_prf_report_ntf_cb(csc_report_ntf_t *report_info)
{
	DBG_LOG("***Received data from device***\r\n");
	csc_app_recv_buf(report_info->recv_buff, report_info->recv_buff_len);
}


/*Initialization function for Ble Observer */

static void ble_scan_init(void)
{
	at_ble_status_t scan_status;

	if (at_ble_scan_stop() != AT_BLE_SUCCESS) {
		/* If scan not started stop scan failed message will be displayed on terminal */
		/* If the scan already started then stop scan will return AT_BLE_SUCCESS */
		DBG_LOG_DEV("Stop scan failed");
	}

	/* Initialize the scanning procedure */
	scan_status = gap_dev_scan();

	/* Check for scan status */
	if (scan_status == AT_BLE_SUCCESS) {
		DBG_LOG("Scanning process initiated");
		} else if (scan_status == AT_BLE_INVALID_PARAM) {
		DBG_LOG("Scan parameters are invalid");
		} else if (scan_status == AT_BLE_FAILURE) {
		DBG_LOG("Scanning Failed Generic error");
	}
}

int main1(void)
{
	platform_driver_init();
	//acquire_sleep_lock();
	bsp_init();
	
	/* Initialize the LED */
	//LED_init();
	
	//sleep_manager_init();
	printf("Inside main1\r\rn");
	/* Initialize serial console */
	//serial_console_init();
	//_usart_async_init(&CONSOLE_IO, UART0);
	//_usart_async_enable(&CONSOLE_IO);

	system_init();

	/* initialize the ble chip  and Set the device mac address */
	ble_device_init(NULL);
	//TIMER_0_start();
	at_ble_whitelist_add(&BlueNRG1);

	ble_scan_init();
	/* Initializing the profile */
	csc_prf_init(NULL);

	/* Register the notification handler */
	notify_recv_ntf_handler(csc_prf_report_ntf_cb);

	/* Register the user event handler */
	register_ble_user_event_cb(csc_app_send_buf);

	//register_uart_callback(uart_rx_callback);

	/* Receiving events */
	while (1) {
		ble_event_task(BLE_EVENT_TIMEOUT);

		/*stdio_io_read(buf, NUM_BYTES);
		
		if(buf[0] == '\r'){
		stdio_io_write((uint8_t *)"\r\n",(NUM_BYTES+1));
		}else{
		stdio_io_write(buf,NUM_BYTES);
		}*/
	}

	return 0;
}

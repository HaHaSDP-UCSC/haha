/**
* \file
*
* \brief Custom Serial Chat Profile
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
* This is the reference manual for the Custom Serial Chat Profile
*/
/*- Includes ---------------------------------------------------------------*/


#include "string.h"
#include "ble_utils.h"
#include "ble_base_manager.h"
#include "bsscp.h"
#include "bsscs.h"
#include "platform.h"
#include "base_station_monitor.h"
#include "ble_base_station.h"
#include "driver_examples.h"
#include "ble_config.h"
#include "app_bsp.h"
#include "driver_init.h"
///* Scan response data */
//uint8_t scan_rsp_data[SCAN_RESP_LEN] = {0x09,0xff, 0x00, 0x06, 0xd6, 0xb2, 0xf0, 0x05, 0xf0, 0xf8};

static const ble_event_callback_t csc_gap_handle[] = {
	NULL,									// 0
	pxp_monitor_scan_data_handler,			// 1
	NULL,									// 2
	NULL,									// 3
	NULL,									// 4
	csc_prf_connected_state_handler,		// 5
	pxp_disconnect_event_handler,			// 6
	NULL,									// 7
	NULL,									// 8
	csc_prf_write_notification_handler,		// 9
	NULL,									// 10
	NULL,									// 11
	NULL,									// 12
	NULL,									// 13
	csc_prf_write_notification_handler,		// 14
	NULL,									// 15
	NULL,									// 16
	NULL,									// 17
	NULL									// 18
};

static const ble_event_callback_t csc_gatt_client_handle[] = {
	csc_prf_service_found_handler,			// 19
	NULL,									// 20
	csc_prf_characteristic_found_handler,	// 21
	NULL,//csc_prf_descriptor_found_handler,		// 22
	csc_prf_discovery_complete_handler,		// 23
	csc_battery_info_handler,				// 24
	NULL,									// 25
	NULL,//cnc_client_write_response_handler,		// 26
	csc_prf_notification_handler,			// 27
	NULL									// 28
};
extern volatile bool button_pressed;
bool write_flag = false;
bool found_flag = false;
bool timer_set = false;
/*Profile Information*/
app_csc_data_t app_csc_info;
at_ble_characteristic_found_t* characteristic[2] = {NULL, NULL};

/* Notification callback function pointer */
recv_ntf_callback_t recv_ntf_cb;
static void button_cb(void);
/* pxp reporter device address to connect */
at_ble_addr_t pxp_reporter_address;

uint8_t pxp_supp_scan_index[MAX_SCAN_DEVICE];
uint8_t scan_index = 0;

extern volatile uint8_t scan_response_count;
extern at_ble_scan_info_t scan_info[MAX_SCAN_DEVICE];

volatile uint8_t pxp_connect_request_flag = PXP_DEV_UNCONNECTED;

//uart_rx_callback_t user_callback_func = NULL;

peripheral_state_cb_t peripheral_state_callback = NULL;

/**
* \CSC buffer initialization function
*/
void csc_prf_buf_init(uint8_t *databuf, uint16_t datalen)
{
	app_csc_info.buff_ptr = databuf;	// So far all I've seen buff_ptr only be used for csc_serv_init, this function should affect much
	app_csc_info.buff_len = datalen;
}
void button_register_callback(ble_button_cb_t button_cb)
{
	ext_irq_register(BLE_APP_SW, button_cb);
}
static void button_cb(void)
{
	at_ble_characteristic_read(app_csc_info.conn_params.handle, app_csc_info.batt_char.char_handle+1, 0, 0);
	DBG_LOG("button pressed");
	button_pressed = true;
}
/**
* \CSC profile initialization function
*/
void csc_prf_init(void *param)
{
	DBG_LOG("*****csc_prf_init*****");
	at_ble_status_t status;
	
	csc_serv_init(app_csc_info.buff_ptr, app_csc_info.buff_len);	// This is defining own service,
																	// but since were trying to use BlueNRG service this not needed
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
	BLE_GAP_EVENT_TYPE,
	csc_gap_handle);
	ble_mgr_events_callback_handler(REGISTER_CALL_BACK,
	BLE_GATT_CLIENT_EVENT_TYPE,
	csc_gatt_client_handle);

	button_register_callback(button_cb);
	status = ble_advertisement_data_set();
	if (status != AT_BLE_SUCCESS) {
		DBG_LOG("Advertisement data set failed reason %d",status);
	}
}

/**@brief Connect to a peer device
*
* Connecting to a peer device, implicitly starting the necessary scan operation
* then connecting if a device in the peers list is found.
*
* @param[in] scan_buffer a list of peers that the device will connect to one of
* them
* @param[in] index index of elements in peers, to initiate the connection
*
* @return @ref AT_BLE_SUCCESS operation programmed successfully
* @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
* @return @ref AT_BLE_FAILURE Generic error.
*/
at_ble_status_t pxp_monitor_connect_request(at_ble_scan_info_t *scan_buffer, uint8_t index)
{
	DBG_LOG("*****pxp_monitor_connect_request*****");
	memcpy((uint8_t *)&pxp_reporter_address, (uint8_t *)&scan_buffer[index].dev_addr,sizeof(at_ble_addr_t));

	if (gap_dev_connect(&pxp_reporter_address) == AT_BLE_SUCCESS) {
		DBG_LOG("PXP Connect request sent");
		pxp_connect_request_flag = PXP_DEV_CONNECTING;
		
		return AT_BLE_SUCCESS;
		} else {
		DBG_LOG("PXP Connect request send failed");
	}

	return AT_BLE_FAILURE;
}

/**@brief Search for a given AD type in a buffer, received from advertising
* packets starts search from the buffer, need to provide required
* search params
*
* @param[in] scan_buffer where all received advertising packet are stored
* @param[in] scanned_dev_count elements in scan_buffer
*
* @return @ref AT_BLE_SUCCESS operation programmed successfully
* @return @ref AT_BLE_INVALID_PARAM incorrect parameter.
* @return @ref AT_BLE_FAILURE Generic error.
*/
at_ble_status_t pxp_monitor_scan_data_handler(void *params)
{
	DBG_LOG("*****pxp_monitor_scan_data_handler*****");
	uint8_t scan_device[MAX_SCAN_DEVICE];
	uint8_t pxp_scan_device_count = 0;
	uint8_t scanned_dev_count = scan_response_count;
	scan_index = 0;
	uint8_t index;
	uint8_t foundindex = 0;
	at_ble_scan_info_t *scan_buffer = (at_ble_scan_info_t *)scan_info;
	memset(scan_device, 0, MAX_SCAN_DEVICE);
	if (scanned_dev_count) {

		for (index = 0; index < scanned_dev_count; index++) {
			/* Display only the connectible devices*/
			if((scan_buffer[index].type == AT_BLE_ADV_TYPE_DIRECTED) || (scan_buffer[index].type == AT_BLE_ADV_TYPE_UNDIRECTED)) {
				scan_device[pxp_scan_device_count++] = index;
			}
		}
		

		
		if (pxp_scan_device_count) {
			/* Address to be searched */
			for (index = 0; index < pxp_scan_device_count; index++) {
				DBG_LOG("Info: Device found address [%d]  0x%02X%02X%02X%02X%02X%02X ",
				index,
				scan_buffer[scan_device[index]].dev_addr.addr[5],
				scan_buffer[scan_device[index]].dev_addr.addr[4],
				scan_buffer[scan_device[index]].dev_addr.addr[3],
				scan_buffer[scan_device[index]].dev_addr.addr[2],
				scan_buffer[scan_device[index]].dev_addr.addr[1],
				scan_buffer[scan_device[index]].dev_addr.addr[0]);
				if(scan_buffer[scan_device[index]].dev_addr.addr[5] == 0x02){
					if(scan_buffer[scan_device[index]].dev_addr.addr[4] == 0x80){
						if(scan_buffer[scan_device[index]].dev_addr.addr[3] == 0xe1){
							if(scan_buffer[scan_device[index]].dev_addr.addr[2] == 0x00){
								if(scan_buffer[scan_device[index]].dev_addr.addr[1] == 0x00){
									if(scan_buffer[scan_device[index]].dev_addr.addr[0] == 0xaa){
										DBG_LOG("Found BlueNRG1, it is in index %d",index);
										foundindex = scan_device[index];
										found_flag = true;
									}
								}
							}
						}
					}
				}
				/*switch(scan_buffer[scan_device[index]].type){
				case AT_BLE_ADV_TYPE_UNDIRECTED:
				DBG_LOG("Device type: AT_BLE_ADV_TYPE_UNDIRECTED");
				break;
				case AT_BLE_ADV_TYPE_DIRECTED:
				DBG_LOG("Device type: AT_BLE_ADV_TYPE_DIRECTED");
				break;
				case AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED:
				DBG_LOG("Device type: AT_BLE_ADV_TYPE_SCANNABLE_UNDIRECTED");
				break;
				case AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED:
				DBG_LOG("Device type: AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED");
				break;
				case AT_BLE_ADV_TYPE_DIRECTED_LDC:
				DBG_LOG("Device type: AT_BLE_ADV_TYPE_NONCONN_UNDIRECTED");
				break;
				case AT_BLE_ADV_TYPE_SCAN_RESPONSE:
				DBG_LOG("Device type: AT_BLE_ADV_TYPE_SCAN_RESPONSE");
				break;
				default:
				DBG_LOG("Device type: OTHER_TYPE");
				}
				for(uint8_t dataidx = 0; dataidx < scan_buffer[scan_device[index]].adv_data_len; dataidx++){
				if(dataidx == 0)
				printf("\r\n0x%02X,",scan_buffer[scan_device[index]].adv_data[dataidx]);
				if((dataidx < scan_buffer[scan_device[index]].adv_data_len) && dataidx != 0){
				printf("0x%02X,",scan_buffer[scan_device[index]].adv_data[dataidx]);
				}else if(dataidx == scan_buffer[scan_device[index]].adv_data_len - 1){
				printf("0x%02X\r\n",scan_buffer[scan_device[index]].adv_data[dataidx]);
				}
				}*/
				//if (scan_info_parse(&scan_buffer[scan_device[index]], &service_uuid,AD_TYPE_COMPLETE_LIST_UUID) ==AT_BLE_SUCCESS) {
				/* Device Service UUID  matched */
				/*		pxp_supp_scan_index[scan_index++] = index;
				DBG_LOG_CONT("---PXP");
				}*/
			}
		}
		/*if (!scan_index)  {
		DBG_LOG("Proximity Profile supported device not found ");
		}*/
		/* Stop the current scan active */
		if(found_flag){
			at_ble_scan_stop();
			return pxp_monitor_connect_request(scan_buffer,	foundindex);
			}else{
			return gap_dev_scan();
		}
		
		
	}
	ALL_UNUSED(params);
	return AT_BLE_FAILURE;
}

at_ble_status_t pxp_monitor_start_scan(void)
{
	DBG_LOG("*****pxp_monitor_start_scan*****");
	if (peripheral_state_callback != NULL)
	{
		if (peripheral_state_callback() == PERIPHERAL_ADVERTISING_STATE)
		{
			DBG_LOG("Peripheral is already Advertising. Scan not permitted");
			return AT_BLE_FAILURE;
		}
	}
	if(found_flag) {
		if (gap_dev_connect(&pxp_reporter_address) == AT_BLE_SUCCESS) {
			DBG_LOG("PXP Re-Connect request sent");
			pxp_connect_request_flag = PXP_DEV_CONNECTING;
			TIMER_0_init();
			TIMER_0_start();
			timer_set = true;
			return AT_BLE_SUCCESS;
			} else {
			DBG_LOG("PXP Re-Connect request send failed");
		}
	}else{ 
		DBG_LOG("Start Scanning");
		return gap_dev_scan();
	}
	return AT_BLE_FAILURE;
}

/**@brief peer device connection terminated
*
* handler for disconnect notification
* try to send connect request for previously connect device.
*
* @param[in] available disconnect handler of peer and
* reason for disconnection
*
* @return @ref AT_BLE_SUCCESS Reconnect request sent to previously connected
*device
* @return @ref AT_BLE_FAILURE Reconnection fails.
*/
at_ble_status_t pxp_disconnect_event_handler(void *params)
{
	DBG_LOG("*****pxp_disconnect_event_handler*****");
	app_csc_info.serv_discovery = false;
	at_ble_disconnected_t *disconnect;
	disconnect = (at_ble_disconnected_t *)params;
	static ble_peripheral_state_t peripheral_state = PERIPHERAL_IDLE_STATE;
	
	if(!ble_check_disconnected_iscentral(disconnect->handle))
	{
		pxp_monitor_start_scan();
		return AT_BLE_FAILURE;
	}else if(peripheral_state_callback != NULL)
	{
		peripheral_state = peripheral_state_callback();
	}
	
	if(peripheral_state != PERIPHERAL_ADVERTISING_STATE)
	{
		if((ble_check_device_state(disconnect->handle, BLE_DEVICE_DISCONNECTED) == AT_BLE_SUCCESS) ||
		(ble_check_device_state(disconnect->handle, BLE_DEVICE_DEFAULT_IDLE) == AT_BLE_SUCCESS))
		{
			if (disconnect->reason == AT_BLE_LL_COMMAND_DISALLOWED) {
				return AT_BLE_SUCCESS;
			} else
			pxp_monitor_start_scan();
		}
	}
	else
	{
		pxp_connect_request_flag = PXP_DEV_UNCONNECTED;
		DBG_LOG("Peripheral is already Advertising,Scan not permitted");
	}

	return AT_BLE_FAILURE;
}

/**
* \CSC profile send data function
*/
void csc_prf_send_data(uint8_t *databuf, uint16_t datalen)
{
	if(at_ble_characteristic_write(app_csc_info.conn_params.handle, (app_csc_info.rx_char.value_handle), 0,datalen, databuf, false, false) == AT_BLE_FAILURE){
		DBG_LOG("\r\nFailed to send Write Request");
		}else{
		DBG_LOG("\r\nSucceded to send Write Request");
	}
}

/**
* @brief Connection handler invoked by ble manager
*/
at_ble_status_t csc_prf_connected_state_handler(void *params)
{
	DBG_LOG("*****csc_prf_connected_state_handler*****");
	at_ble_status_t status;
	memcpy((uint8_t *)&app_csc_info.conn_params, params, sizeof(at_ble_connected_t));
	DBG_LOG("service discovery status: %d",app_csc_info.serv_discovery);
	if(timer_set){
		TIMER_0_stop();
		TIMER_0_deinit();
		timer_set = false;
	}
	if(!app_csc_info.serv_discovery){		// check the service discovery status
		app_csc_info.discover_role = DISCOVER_SERVICE;
		app_csc_info.csc_serv.service_uuid.type = AT_BLE_UUID_128;
		memcpy(&app_csc_info.csc_serv.service_uuid.uuid[0], CSC_SERVICE_UUID, CSC_UUID_128_LEN);
		/* Discover Remote Service by service UUID */
		status = at_ble_primary_service_discover_by_uuid(app_csc_info.conn_params.handle,START_HANDLE, END_HANDLE, &app_csc_info.csc_serv.service_uuid);
		if(status != AT_BLE_SUCCESS){
			DBG_LOG("Failed to start service discovery. status = %d", status);
			} else {
			DBG_LOG("Started service discovery");
			DBG_LOG_DEV("Started service discovery");
		}
	}
	pxp_connect_request_flag = PXP_DEV_CONNECTED;
	return AT_BLE_SUCCESS;
}

/**
* @brief Discovery Complete handler invoked by ble manager
*/
at_ble_status_t csc_prf_discovery_complete_handler(void *params)
{
	DBG_LOG("*****csc_prf_discovery_complete_handler*****");
	
	at_ble_discovery_complete_t discover_status;
	
	memcpy((uint8_t *)&discover_status, params, sizeof(at_ble_discovery_complete_t));
	DBG_LOG("Discover status: %d",discover_status.status);
	
	if(discover_status.status == AT_DISCOVER_SUCCESS){
		if(discover_status.operation == AT_BLE_DISC_BY_UUID_SVC){
			// Printing out start and end handles of connection
			DBG_LOG("Discover Service Info:\r\n -->ConnHandle 0x%02x\r\n -->start handle 0x%02x\r\n -->End handle : 0x%02x",
			app_csc_info.conn_params.handle,
			app_csc_info.csc_serv.start_handle,
			app_csc_info.csc_serv.end_handle);
			DBG_LOG_DEV("Discover Service Info:\r\n -->ConnHandle 0x%02x\r\n -->start handle 0x%02x\r\n -->End handle : 0x%02x",
			app_csc_info.csc_serv.conn_handle,
			app_csc_info.csc_serv.start_handle,
			app_csc_info.csc_serv.end_handle);
			// Discover TX and RX characteristics of BlueNRG1
			if(at_ble_characteristic_discover_all(app_csc_info.conn_params.handle, app_csc_info.csc_serv.start_handle, app_csc_info.csc_serv.end_handle) != AT_BLE_SUCCESS){
				DBG_LOG("Fail to start discover characteristic");
			}
		}
		else if(discover_status.operation == AT_BLE_DISC_ALL_CHAR){

			DBG_LOG("Tx Char handle == %x, addr: %p",(app_csc_info.tx_char.char_handle),&(app_csc_info.tx_char.char_handle));
			DBG_LOG("Rx Char handle == %x, addr: %p",(app_csc_info.tx_char.char_handle),&(app_csc_info.tx_char.char_handle));
			DBG_LOG("Tx Char handle + 2 == %x",(app_csc_info.tx_char.char_handle + 2));
			DBG_LOG("Rx Char handle + 1 == %x",(app_csc_info.tx_char.char_handle) + 1);
			DBG_LOG("Tx Val handle == %x, addr: %p",(app_csc_info.tx_char.value_handle),&(app_csc_info.tx_char.value_handle));
			DBG_LOG("Rx Val handle == %x, addr: %p",(app_csc_info.rx_char.value_handle),&(app_csc_info.rx_char.value_handle));
			DBG_LOG("Tx Val handle + 2 == %x, addr: %p",(app_csc_info.tx_char.value_handle) + 2,&(app_csc_info.tx_char.value_handle));
			DBG_LOG("Rx Val handle + 1 == %x, addr: %p",(app_csc_info.rx_char.value_handle) + 1,&(app_csc_info.rx_char.value_handle));
			app_csc_info.serv_discovery = true;
		}
	}
	return AT_BLE_SUCCESS;
}

/**
* @brief Service found handler invoked by ble manager
*/
at_ble_status_t csc_prf_service_found_handler(void * params)
{

	//DBG_LOG("*****csc_prf_service_found_handler*****");
	DBG_LOG("Service Found handler ");
	DBG_LOG_DEV("Service Found handler, service UUID follows: ");
	
	// Storing the BlueNRG1's primary service info (i.e. UUID) into app_csc_info.csc_serv
	memcpy((uint8_t *)&app_csc_info.csc_serv, params, sizeof(at_ble_primary_service_found_t));
	for(int i = 0; i < 16; i++){
		printf(" 0x%02X, ",app_csc_info.csc_serv.service_uuid.uuid[i]);
	}
	return AT_BLE_SUCCESS;
}

/**
* @brief characteristic found handler invoked by ble manager
*/
at_ble_status_t csc_prf_characteristic_found_handler(void *params)
{
	uint16_t charac_16_uuid = 0;
	at_ble_characteristic_found_t* characteristic_found;
	characteristic_found = (at_ble_characteristic_found_t*)params;
	//uint8_t data[2] = {1,0};
	if((characteristic_found->char_uuid.uuid[12] == 0xe1) && (characteristic_found->char_uuid.uuid[13] == 0xf2)){
		charac_16_uuid = characteristic_found->char_uuid.uuid[12] | (characteristic_found->char_uuid.uuid[13] << 8);
		memcpy((uint8_t *)&app_csc_info.tx_char, params, sizeof(at_ble_characteristic_found_t));
	}
	if((characteristic_found->char_uuid.uuid[12] == 0xe2) && (characteristic_found->char_uuid.uuid[13] == 0xf2)){
		charac_16_uuid = characteristic_found->char_uuid.uuid[12] | (characteristic_found->char_uuid.uuid[13] << 8);
		memcpy((uint8_t *)&app_csc_info.rx_char, params, sizeof(at_ble_characteristic_found_t));
	}
	if((characteristic_found->char_uuid.uuid[12] == 0xe3) && (characteristic_found->char_uuid.uuid[13] == 0xf2)){
		charac_16_uuid = characteristic_found->char_uuid.uuid[12] | (characteristic_found->char_uuid.uuid[13] << 8);
		memcpy((uint8_t *)&app_csc_info.batt_char, params, sizeof(at_ble_characteristic_found_t));
	}
	if(charac_16_uuid == TX_CHAR_16_UUID){
		DBG_LOG("TX characteristics: Attrib handle %x Char handle %x w/ addr: %p property %d handle: %x uuid : %x",
		characteristic_found->char_handle, app_csc_info.tx_char.char_handle, &(app_csc_info.tx_char.char_handle),characteristic_found->properties, app_csc_info.tx_char.char_handle, charac_16_uuid);
	}else if(charac_16_uuid == RX_CHAR_16_UUID){
		DBG_LOG("RX characteristics: Attrib handle %x Char handle %x w/ addr: %p property %d handle: %x uuid : %x",
		characteristic_found->char_handle, app_csc_info.rx_char.char_handle, &(app_csc_info.rx_char.char_handle),characteristic_found->properties, app_csc_info.rx_char.char_handle, charac_16_uuid);
	}else if(BATT_CHAR_16_UUID){
		DBG_LOG("Battery characteristics: Attrib handle %x Char handle %x w/ addr: %p property %d handle: %x uuid : %x",
		characteristic_found->char_handle, app_csc_info.batt_char.char_handle, &(app_csc_info.batt_char.char_handle),characteristic_found->properties, app_csc_info.batt_char.char_handle, charac_16_uuid);
	}
	return AT_BLE_SUCCESS;
}

/**
* @brief client descriptor found handler invoked by ble manager
*/
/*at_ble_status_t csc_prf_descriptor_found_handler(void *params)
{
	DBG_LOG("*****csc_prf_descriptor_found_handler*****");
	memcpy((uint8_t *)&app_csc_info.csc_desc, params, sizeof(at_ble_descriptor_found_t));
	
	DBG_LOG("Descriptor: Attrib handle %x Char handle %x: uuid",
	app_csc_info.csc_desc.conn_handle, app_csc_info.csc_desc.desc_handle);
	for(int i = 0; i < 16; i++){
		printf(" 0x%02X, ",app_csc_info.csc_desc.desc_uuid.uuid[i]);
	}
	
	return AT_BLE_SUCCESS;
}*/

/**
* @brief invoked by ble manager on receiving notification
*/
at_ble_status_t csc_prf_notification_handler(void *params)
{
	DBG_LOG("*****csc_prf_notification_handler*****");
	at_ble_notification_recieved_t notif;
	csc_report_ntf_t ntf_info;
	memcpy((uint8_t *)&notif, params, sizeof(at_ble_notification_recieved_t));
	ntf_info.conn_handle = notif.conn_handle;
	ntf_info.recv_buff_len = notif.char_len;
	ntf_info.recv_buff = &notif.char_value[0];
	recv_ntf_cb(&ntf_info);
	return AT_BLE_SUCCESS;
}

/**
* @brief invoked by ble manager for setting the notification
*/
at_ble_status_t csc_prf_write_notification_handler(void *params)
{
	DBG_LOG("*****csc_prf_write_notification_handler*****");
	uint8_t data[2] = {1,0};
	DBG_LOG("\r\n");
	write_flag = true;
	if(at_ble_characteristic_write(app_csc_info.conn_params.handle, ((app_csc_info.tx_char.char_handle) + 2), 0,2, data, false, false) == AT_BLE_FAILURE){
		DBG_LOG("\r\nFailed to send characteristic Write Request");
	}
	usart_sync_enable(&CONSOLE_UART);
	return AT_BLE_SUCCESS;
}

/**
* \Service characteristic change handler function
*/
at_ble_status_t csc_prf_char_changed_handler(void *params)
{
	at_ble_characteristic_changed_t change_params;
	memcpy((uint8_t *)&change_params, params, sizeof(at_ble_characteristic_changed_t));
	return AT_BLE_SUCCESS;
}

/**
* \Notify to user about the new data received
*/
void notify_recv_ntf_handler(recv_ntf_callback_t recv_ntf_fn)
{
	recv_ntf_cb = recv_ntf_fn;
}


/*
void register_uart_callback(uart_rx_callback_t callback_func)
{
	user_callback_func = callback_func;
	uart_enable_callback(&uart_instance, UART_RX_COMPLETE);
	uart_read_buffer_job(&uart_instance, string_input, sizeof(string_input));
}
*/
/** @brief csc_notification_confirmation_handler called by ble manager
*	to give the status of notification sent
*  @param[in] at_ble_cmd_complete_event_t address of the cmd completion
*/
at_ble_status_t csc_notification_confirmation_handler(void *params)
{
	DBG_LOG("*****csc_notification_confirmation_handler*****");
	at_ble_cmd_complete_event_t *event_param = (at_ble_cmd_complete_event_t *)params;
	if (event_param->status == AT_BLE_SUCCESS){

		DBG_LOG("Notification Successfully sent over the air");
		DBG_LOG_DEV("\r\n");
		} else {
		DBG_LOG("Sending Notification over the air failed");
		DBG_LOG_DEV("Sending Notification over the air failed");
		DBG_LOG_DEV("\r\n");
	}
	return AT_BLE_SUCCESS;
}
at_ble_status_t csc_battery_info_handler(void *params)
{
	at_ble_characteristic_read_response_t batt_data;
	memcpy((uint8_t *)&batt_data, params, sizeof(at_ble_characteristic_read_response_t)); 
	DBG_LOG("0xX%02X%02X%02X",batt_data.char_value[3],batt_data.char_value[2],batt_data.char_value[1]);
	return AT_BLE_SUCCESS;
}
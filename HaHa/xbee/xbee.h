#ifndef HA_XBEE
#define HA_XBEE
/*
 * xbee.h
 *
 * Created: 3/5/2017 6:03:35 PM
 *  Author: Brian
 */ 

#include "haha/network.h"
#include <stdio.h>

/* Configure XBee */
void xbee_init();
void xbee_enterCmdMode();
void xbee_leaveCmdMode();
//Set into API Mode
uint8_t xbee_setAPI(uint8_t type);
//Get own MAC (64-bit address)
uint8_t xbee_getOwnMacHigh();
uint8_t xbee_getOwnMacLow();
//Get own Net Address
uint8_t xbee_getOwnNetHigh();
uint8_t xbee_getOwnNetLow();

uint8_t xbee_send(char* dst, char* data);
uint8_t xbee_recv(char* data, uint8_t len);


/* Packets */
uint8_t send_packet(char* p);
uint8_t gen_checksum(char* data);

//Helper
uint8_t asciihex_to_byte(uint8_t t, uint8_t);



/********** Received Packets **********/
/* Receive Options */
/* Note: Other bits not used */
#define OPT_DIGIMESH	0x11000000
#define OPT_PACKETACK	0x00000001
#define OPT_PACKETBCAST	0x00000010

//typedef struct {
	//uint8_t 	start;
	//uint16_t 	length; /* total in frame's data field */
	//uint8_t 	frameType;
	//uint8_t 	src[8];
	//uint8_t 	res[2];
	//uint8_t 	opt;
	//uint8_t		data[100];
	//uint8_t		checksum;
//}rxPacketXbee;















 #endif
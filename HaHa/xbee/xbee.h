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

/* Send and Receive */
uint8_t xbee_send(char* dst, char* data, uint8_t len);
uint8_t xbee_recv(char* data, uint8_t len);


/* Packets */
uint8_t send_packet(char* p);
uint8_t gen_checksum(char* data);



/********** FRAMES **********/
/*
StartDelimiter(1B) + Length(2B) +  Frame Data(variable) + Checksum(1B)
*  ______________     ___________     __________________     __________
* |              |   |     |     |   |                  |   |          |
* |     0x7E     | + | MSB | LSB | + |    Frame Data    | + |  1 Byte  |
* |______________|   |_____|_____|   |__________________|   |__________|
*/

/* Receive Options */
/* Note: Other bits not used */
#define OPT_DIGIMESH	0x11000000
#define OPT_PACKETACK	0x00000001
#define OPT_PACKETBCAST	0x00000010

/* CallBacks */
typedef void (*xbee_cb_t)(char *, uint8_t);
void xbee_register_callback(xbee_cb_t t);
/*CallBack Table


/* RX */
#define FRAME_RX_HEAD_LEN 12
typedef struct {
    uint16_t 	data_length; /* total in frame's data field */
    uint8_t		frametype;
    uint8_t 	src[8];
    uint8_t 	res[2];
    uint8_t 	opt;
    uint8_t		data[100];
    uint8_t		checksum;
    uint8_t		payload_length;
    xbee_cb_t	callback;
}frameRX;

/* TX */
#define FRAME_TX_HEAD_LEN 14 /* Length of tx frame header info */
typedef struct {
    uint16_t data_len;
    uint8_t frametype;
    uint8_t frameid;
    uint8_t dst[8]; //change to char* -- no just make a wrapper function
    uint8_t res[2]; //chat to char*
    uint8_t b_rad;
    uint8_t opt;
    char*	data;
    uint8_t checksum;
}frameTX;















 #endif
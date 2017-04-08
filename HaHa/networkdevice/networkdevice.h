/**
 * \file
 *
 * \brief XBee Network device implementation
 */
#ifndef HA_XBEE
#define HA_XBEE
/*
 * xbee.h
 *
 * Created: 3/5/2017 6:03:35 PM
 *  Author: Brian
 */ 

#include "network/network.h"
#include <stdio.h>

/**
 * @brief      { Intialize the XBee module }
 */
void xbee_init();
/**
 * @brief      { Enter XBee command mode }
 */
void xbee_enterCmdMode();
/**
 * @brief      { Leave XBee command mode }
 */
void xbee_leaveCmdMode();

/**
 * @brief      { Set XBee to API Mode }
 *
 * @param[in]  type  API Mode [1 or 2]
 *
 * @return     { returns 0 if successful }
 */
uint8_t xbee_setAPI(uint8_t type);


/**
 * @brief      { Network address high bytes}
 *
 * @return     { Network address high bytes }
 */
uint8_t xbee_getOwnNetHigh();
/**
 * @brief      { Network address low bytes}
 *
 * @return     { Network address low bytes }
 */
uint8_t xbee_getOwnNetLow();

/**
 * @brief      { Send a packet to another XBee node }
 *
 * @param      dst   The destination
 * @param      data  The data
 * @param[in]  len   The length
 *
 * @return     { returns 0 if sucessful }
 */
uint8_t xbee_send(char* dst, char* data, uint8_t len);
/**
 * @brief      { Receives UART data and parses into frame packets}
 *
 * @param      data  The data
 * @param[in]  len   The length
 *
 * @return     { returns 0 if packet is processed }
 */
uint8_t xbee_recv(char* data, uint8_t len);
/**
 * @brief      { Calculate the XBee checksum }
 *
 * @param      data  The data
 *
 * @return     { returns 0 on success }
 */
uint8_t gen_checksum(char* data);



/********** FRAMES **********/
/*
* FrameData[0] contains frame type
StartDelimiter(1B) + Length(2B) +  Frame Data(variable) + Checksum(1B)
*  ______________     ___________     __________________     __________
* |              |   |     |     |   |                  |   |          |
* |     0x7E     | + | MSB | LSB | + |    Frame Data    | + |  1 Byte  |
* |______________|   |_____|_____|   |__________________|   |__________|
*/

/* Receive Options */
/* Note: Other bits not used */
/**
 * { XBee TX frame send type: DigiMesh }
 */
#define OPT_DIGIMESH	0x11000000
/**
 * { XBee TX frame send type: Packet Acknowledgement }
 */
#define OPT_PACKETACK	0x00000001
/**
 * { XBee TX frame send type: Packet Broadcast }
 */
#define OPT_PACKETBCAST	0x00000010

/* CallBacks */
/* Frames to XBee */
typedef enum {	
    FRAME_AT = 0x08,
    FRAME_AT_QUEUE = 0x09,
    FRAME_TX = 0x10,
    FRAME_TX_EXPLICIT = 0x11,
    FRAME_REMOTE_COMMAND = 0x17
}frameRequestType;

/* Frames from XBee */
typedef enum {	
    FRAME_AT_RESPONSE = 0x88,
    FRAME_MODEM_STATUS = 0x8A,
    FRAME_TX_STATUS = 0x8B,
    FRAME_ROUTE_INFO = 0x8D,
    FRAME_AGG_ADDR = 0x8E,
    FRAME_RX = 0x90, //only for AO=0
    FRAME_RX_EXPLICIT = 0x91, //only for AO=1
    FRAME_DATA_SAMPE = 0x92,
    FRAME_NODE_ID = 0x95,
    FRAME_REMOTE_RESP = 0x97
}frameResponseType;
/**
 * { Xbee callback function type }
 */
typedef void (*xbee_cb_t)(char *, uint8_t);
/**
 * @brief      { Register the function to send received payload }
 *
 * @param[in]  t     { Function to call when app data is received }
 */
void xbee_register_callback(xbee_cb_t t, frameResponseType type);

/* Incoming Frames from Xbee */
/**
 * { XBee generic frame packet }
 */
typedef struct {
    uint16_t 	data_length; /* total in frame's data field, use for checksum */
    uint16_t 	real_length; /* total in data field minus the frametype */
    uint8_t		frametype;
    uint8_t		data[150];
    uint8_t		checksum;
}frameIncoming;


/* RX */
/**
 * { Length of RX frame header fields }
 */
#define FRAME_RX_HEAD_LEN 12
/**
 * { XBee RX frame packet }
 */
typedef struct {
    //uint16_t 	data_length; /* total in frame's data field */
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
/**
 * { Length of TX frame header fields }
 */
#define FRAME_TX_HEAD_LEN 14 /* Length of tx frame header info */
/**
 * { XBee TX frame packet }
 */
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
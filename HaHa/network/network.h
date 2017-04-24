/*
 * @file network.h
 * @brief Definition of different network layers. Net data passed through this.
 * @author Kevin Lee, Brian Nichols
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef _HA_NETWORK
#define _HA_NETWORK

#include "utils/hahaUtils.h"
#include "packet.h"

#define netaddr char *//TODO @brian define better to comply with xbee
#define MAX_NET_ARRAY 10
typedef struct {
	//TODO @brian Network information to send/receive packet. May need more.
	opcode opcode;
	netaddr src;
	netaddr dest;
	bool broadcast;
	uint8_t ttl;
} Network;

unsigned int netIDLast;
Network NET_ARRAY[MAX_NET_ARRAY]; //TODO @brian@kevin what is this for?

bool networkStatus(); //TODO flesh out.

void network_init();

#endif /* NETWORK_H_ */
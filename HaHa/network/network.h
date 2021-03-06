/*
 * @file network.h
 * @brief Definition of different network layers. Net data passed through this.
 * @author Kevin Lee, Brian Nichols
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef _HA_NETWORK
#define _HA_NETWORK

#include "utils/hahaUtils.h"
//#include "packet.h"

#define netaddr char*
#define MAX_NET_ARRAY 10
#define NOT_FOUND -1

netaddr myNetAddr;
typedef struct {
	netaddr src; //The source of the packet.
	netaddr dest; //The destination of the packet.
	bool broadcast;
	uint8_t ttl;
    char *data;
    uint16_t len;
    uint8_t id;
} Network;

unsigned int netIDLast;
Network NET_ARRAY[MAX_NET_ARRAY];

bool networkStatus(); //TODO flesh out.

void network_init();

void netArrayAdd(Network* add);
/* Return index of net array with id netID */
uint8_t netArrayReturn(uint8_t netID);
bool netCompare(netaddr n1, netaddr n2);

void printNetAddr(netaddr t);

#endif /* NETWORK_H_ */
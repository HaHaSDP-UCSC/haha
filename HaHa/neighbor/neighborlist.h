/*
 * @file neighborlist.h
 * @brief List of Neighbors with accompanying data.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef NEIGHBORLIST_H_
#define NEIGHBORLIST_H_

#include "utils/hahaUtils.h"
#include "messagequeue/packet.h"

#define NEIGHBORLISTSIZE 256

typedef struct {
	unsigned char networkaddr[MAXNETADDR]; //Network Address.
	uint16_t port; //Network port.
	int lastresponse; //32-bit offset from boot timer.
	ttl hops; //The time to live before a packet is dropped.
	uint8_t neighborFlags; //8 Available flags for expected responses.
} Neighbor;

Neighbor neighborList[NEIGHBORLISTSIZE];

//TODO @kevin @brian storage for NeighborList.
bool initNeighborList();
bool writeToNeighborListStorage(); //TODO internal, may not be necessary.
bool readFromNeighborListStorage();  //TODO internal, may not be necessary.

#endif /* NEIGHBORLIST_H_ */
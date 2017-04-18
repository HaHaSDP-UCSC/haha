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

#define NEIGHBORLISTSIZE 32

typedef struct {
	//uint8_t id; //Internal ID.
	char networkaddr[MAXNETADDR]; //TODO Finalize data structure.
	uint16_t port; //Network port.
	long lastresponse; //TODO time data structure
	ttl hops;
	BYTE neighborFlags; //8 Available flags for expected responses.
} Neighbor;

Neighbor neighborList[NEIGHBORLISTSIZE];

//TODO @kevin @brian storage for FriendList.
bool initNeighborList();
bool writeToNeighborListStorage(); //TODO internal, may not be necessary.
bool readFromNeighborListStorage();  //TODO internal, may not be necessary.

#endif /* NEIGHBORLIST_H_ */
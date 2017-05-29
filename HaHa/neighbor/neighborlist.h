/*
 * @file neighborlist.h
 * @brief List of Neighbors with accompanying data.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef NEIGHBORLIST_H_
#define NEIGHBORLIST_H_

#include "utils/hahaUtils.h"
#include "network/network.h"
#include "network/packet.h"


#define NEIGHBORLISTSIZE 256 //TODO make this an option, also smaller.

typedef struct Neighbor {
	unsigned char networkaddr[MAXNETADDR]; //Network Address.
	uint16_t port; //Network port.
	int lastresponse; //32-bit offset from boot timer.
	ttl hops; //The time to live before a packet is dropped.
	uint8_t neighborFlags; //8 Available flags for expected responses.
} Neighbor;

Neighbor neighborList[NEIGHBORLISTSIZE]; //TODO perhaps make this hidden in c file.
int numNeighbors;

//TODO @kevin @brian storage for NeighborList.
bool initNeighborList();
bool writeToNeighborListStorage(); //TODO internal, may not be necessary.
bool readFromNeighborListStorage();  //TODO internal, may not be necessary.

bool updateNeighborList(); /* Updates the list. */
bool addNeighbor(Packet *p, Network *net, int currentTime); //Add neighbor if they respond to neighbor request.
bool removeNeighbor(int neighborNumber); //Remove Neighbor
bool checkForNeighbor(Network *net); //Checks for specified neighbor.

#endif /* NEIGHBORLIST_H_ */
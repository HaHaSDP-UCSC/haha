/*
 * @file neighborlist.c
 * @brief List of Neighbors with accompanying data implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "neighborlist.h"

int numNeighbors = 0;

/************************************************************************/
/* INTERNAL METHODS                                                     */
/************************************************************************/

/**
 * @brief      Writes to neighbor list to storage.
 *
 * @return     true if successful, false otherwise.
 */
bool writeToNeighborListStorage(Neighbor neighbor) {
	return false; //TODO Implement
}

/**
 * @brief      Reads neighbors from neighbor list storage.
 *
 * @return     true if successful, false otherwise.
 */
bool readFromNeighborListStorage(Neighbor neighbor) {
	return false; //TODO Implement
}

/************************************************************************/
/* EXTERNAL METHODS                                                     */
/************************************************************************/

/**
 * @brief      Initializes the neighbor list.
 *
 * @return     true if successful, false otherwise.
 */
bool initNeighborList() {
	//bzero(neighborList, sizeof(NeighborList));
	for (int i = 0; i < NEIGHBORLISTSIZE; i++) {
		bzero(&neighborList[i], sizeof(Neighbor)); //Clear up list. TODO Is this proper way to clean struct
	}
	return true;
}

/**
 * @brief      Updates the entire neighbor list.
 *
 * @return     true if successful, false otherwise.
 */
bool updateNeighborList() {
	initNeighborList(); //Clear list first.
	//TODO send packet command to get info from everyone.
	Friend f;
	//f.networkaddr = BROADCASTADDR;
	LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
	send_find_neighbors_request(&f, &self);
	return false; //TODO Implement
}

/**
 * @brief      Adds a neighbor to the list if they responded
 * 			   to a neighbor request.
 *
 * @param[in] net Network information.
 * 
 * @return     true if successful, false otherwise.
 */
bool addNeighbor(Packet *p, Network *net, int currentTime) {
	neighborList[numNeighbors].hops = net->ttl;
	neighborList[numNeighbors].lastresponse = currentTime;
	neighborList[numNeighbors].neighborFlags = 0; //TODO fix
	memcpy(neighborList[numNeighbors].networkaddr, net->src, MAXNETADDR); //TODO @brian is this correct src? want to send to other station.
	//neighborList->port = p->SRCUID;
	neighborList[numNeighbors].port = p->ORIGINUID;
	strcpy(neighborList[numNeighbors].firstname, p->SRCFIRSTNAME);
	strcpy(neighborList[numNeighbors].lastname, p->SRCLASTNAME);
	numNeighbors++;
	numNeighbors = numNeighbors % NEIGHBORLISTSIZE; //Overwrite pre-existing neighbors. Failsafe //TODO do better
	return true;
}

/**
 * @brief      Removes a neighbor from the list.
 * 
 * @param[in] neighborNumber Number in list.
 * 
 * @return     true if successful, false otherwise.
 */
bool removeNeighbor(int neighborNumber) {
	bzero(neighborList[neighborNumber].networkaddr, MAXNETADDR);
	//Reshuffle neighbors up.
	for (int j = neighborNumber; j < FRIENDLISTSIZE-neighborNumber-1; j++) {
		neighborList[j] = neighborList[j+1];
	}
	numNeighbors--;
	return true;
}

bool checkForNeighbor(Network *net) {
	for (int i = 0; i < neighborList; i++) {
		if (!strcmp(net, neighborList[i].networkaddr)) {
			//Found the neighbor by Network Address.
			return true;
		}
	}
	return false;
}
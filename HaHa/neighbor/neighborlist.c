/*
 * @file neighborlist.c
 * @brief List of Neighbors with accompanying data implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "neighborlist.h"


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
	return false; //TODO Implement	
}

/**
 * @brief      Updates the entire neighbor list.
 *
 * @return     true if successful, false otherwise.
 */
bool updateNeighborList() {
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
bool addNeighbor(Network net, int currentTime) {
	//TODO needs more parameters
	return false; //TODO Implement
}

/**
 * @brief      Removes a neighbor from the list.
 * 
 * @param[in] neighborNumber Number in list.
 * @param[in] lastResponse   Checks if last response is overdue.
 * 
 * @return     true if successful, false otherwise.
 */
bool removeNeighbor(int neighborNumber, int lastResponse) {
	return false; //TODO Implement
}
/*
 * IncFile1.h
 *
 * Created: 4/8/2017 11:16:20 PM
 *  Author: kevin
 */ 

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

typedef struct {
	uint8_t id; //Internal ID.
	char firstname[FIRSTNAME];
	char lastname[LASTNAME];
	char networkaddr[NETADDR]; //TODO Finalize data structure.
	uint16_t port; //Network port.
	long lastresponse; //TODO time data structure
	uint16_t responseflag; //16 Available flags for expected responses.
} Friend;




#endif /* FRIENDLIST_H_ */
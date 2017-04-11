/*
 * IncFile1.h
 *
 * Created: 4/8/2017 11:16:20 PM
 *  Author: kevin
 */ 

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

#include "utils/hahaUtils.h"

#define MAXFIRSTNAME 17  // Null terminated
#define MAXLASTNAME 17   // Null terminated
#define MAXPORT 2        // Null terminated

#define MAXNETADDR 75   // Null terminated
#define MAXHOMEADDR 75   // Null terminated
#define MAXPHONE 16      // Null terminated

typedef struct {
	uint8_t id; //Internal ID.
	char firstname[MAXFIRSTNAME];
	char lastname[MAXLASTNAME];
	char networkaddr[MAXNETADDR]; //TODO Finalize data structure.
	uint16_t port; //Network port.
	long lastresponse; //TODO time data structure
	uint16_t responseflag; //16 Available flags for expected responses.
} Friend;




#endif /* FRIENDLIST_H_ */
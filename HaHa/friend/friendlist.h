/*
 * @file friendlist.h
 * @brief List of friends with accompanying data.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

#define MAXFIRSTNAME 17
#define MAXLASTNAME 17
#define MAXNETADDR 0 //TODO fix.
#define MAXPORT 2

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
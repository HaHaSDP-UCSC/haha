/*
 * messagequeue.h
 * A queue of expected messages to be serviced.
 * Created: 4/8/2017 4:37:47 PM
 *  Author: kevin
 */ 

#define MAXQUEUESIZE 256

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

typedef struct {
	uint8_t id; //Internal Friend ID, if this is a friend.
	char networkAddr[NETADDR]; //TODO Finalize Data Structure.
	uint16_t port; //Network port.
	long expiration; //TODO Time structure.
	uint8_t messagetype; //Expected Response.
	} Message;

Message messageQueue[MAXQUEUESIZE];

#endif /* MESSAGEQUEUE_H_ */
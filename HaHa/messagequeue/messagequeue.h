/*
 * messagequeue.h
 * A queue of expected messages to be serviced.
 * Created: 4/8/2017 4:37:47 PM
 *  Author: kevin
 */ 

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "utils/hahaUtils.h"
#include "friend/friendlist.h"
#include "neighbor/neighborlist.h"

#define MAXQUEUESIZE 256

typedef struct {
	uint8_t id; //Internal Friend ID, if this is a friend.
	char networkAddr[MAXNETADDR]; //TODO Finalize Data Structure.
	uint16_t port; //Network port.
	long expiration; //TODO Time structure.
	uint8_t messagetype; //Expected Response.
	} Message;

Message messageQueue[MAXQUEUESIZE];

#endif /* MESSAGEQUEUE_H_ */
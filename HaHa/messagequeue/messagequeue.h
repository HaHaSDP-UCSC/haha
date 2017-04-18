/*
 * @file messagequeue.h
 * @brief A queue of expected messages to be serviced.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "utils/hahaUtils.h"
#include "friend/friendlist.h"
#include "neighbor/neighborlist.h"

#define MAXQUEUESIZE 256

typedef struct {
	uint8_t id; //Internal Friend ID, if this is a friend.
	char networkAddr[MAXNETADDR]; //TODO @brian@kevin Finalize Data Structure.
	uint16_t port; //Network port.
	long expiration; //TODO Time structure.
	opcode opcode; //Expected Response.
	} Message;

Message messageQueue[MAXQUEUESIZE];

bool messageQueueInit(Friend friendlist[]); //Initializes the message queue.
bool addToQueue(Message mes); //Add message to queue.
bool removeFromQueue(Message mes); //TODO Internal //Delete message from queue.
bool checkQueue(Message mes); 
/*Checks queue for a message match. If match and not permanent, delete.*/

//Internal //TODO move this to the c file.
bool generateMessage(Friend friend, bool permanent, Message *mes); //Generate message.
bool initMessageQueuePermanentMessages(); //Loads permanent messages into queue.
//TODO @kevin @brian^^alternative to this would be checking opcode to see if automatic response.

#endif /* MESSAGEQUEUE_H_ */
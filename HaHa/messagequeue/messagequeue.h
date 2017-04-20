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
	unsigned char networkAddr[MAXNETADDR];  //Network Address.
	uint16_t port; //Network port.
	int expiration; //Expiration time.
	opcode opcode; //Expected Packet Response.
	} Message;

Message messageQueue[MAXQUEUESIZE];

int queueTime = 0;

bool initMessageQueue(); //Initializes the message queue.
bool addToQueue(Message mes); //Add message to queue.
bool removeFromQueue(Message mes); //Delete message from queue.
bool checkQueue(); /* Checks queue for a message match. */
bool flushOldMessages(); /* Checks for old messages to be deleted. */

//Internal //TODO move this to the c file.
/*
bool generateMessage(Friend friend, bool permanent, Message *mes); //Generate message.
bool initPermanentMessages(); //Loads permanent messages into queue.
//TODO @kevin @brian^^alternative to this would be checking opcode to see if automatic response.
bool initPendingMessages(); //Initializes messages from friends into the queue.
*/
#endif /* MESSAGEQUEUE_H_ */
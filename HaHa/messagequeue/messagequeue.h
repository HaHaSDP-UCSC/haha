/*
 * @file messagequeue.h
 * @brief A queue of expected messages to be serviced.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef MESSAGEQUEUE_H_
#define MESSAGEQUEUE_H_

#include "utils/hahaUtils.h"
#include "neighbor/friendlist.h"
#include "neighbor/neighborlist.h"

#define MAXQUEUESIZE 256
#define DEFAULT_TTL 1000; //TODO: figure out approrpiate number

typedef struct Message {
	uint8_t id; //Internal Friend ID, if this is a friend.
	char networkAddr[MAXNETADDR];  //Network Address.
	//uint16_t port; //Network port.
    uint16_t srcid;
	int expiration; //Expiration time.
	opcode opcode; //Expected Packet Response.
} Message;

Message messageQueue[MAXQUEUESIZE];

int queueTime;

bool initMessageQueue(); //Initializes the message queue.
bool addToQueue(Message* mes); //Add message to queue.
bool removeFromQueue(int queuenumber); //Delete message from queue.
bool checkQueue(Message* mes);/* Checks queue for a message match. */
bool flushOldMessages(); /* Checks for old messages to be deleted. */
bool initMessage(Message* m); /* Initialize a blank Message */

//Internal //TODO move this to the c file.
/*
bool generateMessage(Friend friend, bool permanent, Message *mes); //Generate message.
bool initPermanentMessages(); //Loads permanent messages into queue.
//TODO @kevin @brian^^alternative to this would be checking opcode to see if automatic response.
bool initPendingMessages(); //Initializes messages from friends into the queue.
*/
#endif /* MESSAGEQUEUE_H_ */
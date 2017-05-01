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

#define DEFMESSAGETIMEOUT 300 //300 ticks, 5 minutes
#define BROADCASTHOP 64 //Maximum neighbor devices to explore.

typedef struct Message {
	opcode opcode; //Expected Packet Response.
	uint8_t id; //Internal Friend ID, if this is a friend.
	bool permanent; //If permanent, does not check expiration date.
	bool broadcast; //If broadcast, does not check network address.
	uint32_t expiration; //Expiration time.
	char srcAddr[MAXNETADDR]; //Source Network Address.
	uint16_t srcid;
	uint8_t numUses; //How many times can be used before it expires (BRDCST)
} Message;

typedef struct {
	//opcode opcode;
	char srcAddr[MAXNETADDR];
	uint16_t srcid;
} Event;

uint32_t queueTime; //System timer. Driven by a super timer //TODO create super timer

bool initMessageQueue(); //Initializes the message queue.
bool addToQueue(Message *mes); //Add message to queue.
bool removeFromQueue(int queuenumber); //Delete message from queue.
int checkQueue(opcode op, Network *net, Event eventList[]); /* Checks queue for a message match. */
bool flushOldMessages(); /* Checks for old messages to be deleted. */

//Internal //TODO move this to the c file.
/*
bool generateMessage(Friend friend, bool permanent, Message *mes); //Generate message.
bool initPermanentMessages(); //Loads permanent messages into queue.
//TODO @kevin @brian^^alternative to this would be checking opcode to see if automatic response.
bool initPendingMessages(); //Initializes messages from friends into the queue.
*/
#endif /* MESSAGEQUEUE_H_ */
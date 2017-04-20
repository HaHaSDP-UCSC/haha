/*
 * @file messagequeue.c
 * @brief A queue of expected messages to be serviced implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "messagequeue.h"

/* Generates a message based on friend parameters. */
/* Helper Function */
bool generateMessage(Friend friend, bool permanent, Message *mes) {
	return false; //TODO implement
}

/* Initializes the message queue with pending friend messages.*/
bool initPendingMessages() {
	return false; //TODO implement
}

/* Initializes permanent messages into the queue. */
bool initPermanentMessages() {
	return false; //TODO implement
}

/* Initializes the message queue. */
bool initMessageQueue() {
	initPermanentMessages();
	initPendingMessages();
	return false; //TODO implement
}

/* Adds message to the queue. */
bool addToQueue(Message mes) {
	return false; //TODO implement
}

/* Removes message from the queue. */
bool removeFromQueue(Message mes) {
	return false; //TODO implement
}

/* Checks if message is in the queue. */
/* Parameters to compare: opcode, id, Network Address, port, */
/*Checks queue for a message match. If match and not permanent, delete.*/
bool checkQueue(Message mes) {
	return false; //TODO implement
}

/* Checks for old messages to be deleted. */
bool flushOldMessages() {
	return false; //TODO implement
}
/*
* @file messagequeue.c
* @brief A queue of expected messages to be serviced implementation.
* @author Kevin Lee
* @date 4/8/2017 4:37:47 PM
*/

#include "messagequeue.h"
#include <string.h>

/************************************************************************/
/* INTERNAL METHODS                                                     */
/************************************************************************/

/**
* @brief      Generates a message based on friend parameters.
*
* @param[in]  Friend	  Input friend
* @param[in]  permanent  The permanent tag to the message.
* @param      mes        A pointer to the message to be created.
*
* @return     true if successful, false otherwise, Message mes.
*/
bool generateFriendMessage(Friend *friend, bool permanent, Message *mes) {
	//friend->id = 0;
	return false; //TODO implement
}

/**
* @brief      Generates a normal message to allow into the message queue.
*
* @param      mes        A pointer to the message to be created.
* @param[in]  broadcast  Any address is accepted.
* @param[in]  permanent  The permanent tag to the message.
*
* @return     true if successful, false otherwise, Message mes.
*/
bool generateMessage(Message *mes, bool broadcast, bool permanent, opcode op,
uint32_t timeout, netaddr addr, uint16_t srcuid) {
	if (op > MAX_OPCODE) {
		printe("Message has invalid opcode.\n");
		return false;
	}
	mes->opcode = op;
	mes->id = FRIENDLISTSIZE; //Not a friend, out of boundary.
	mes->broadcast = broadcast;
	if (permanent) {
		mes->permanent = true;
		} else {
		mes->expiration = queueTime + timeout;
	}
	if (strlen(addr) > MAXNETADDR) {
		printe("Network address too large.\n");
		return false;
	}
	strcpy(mes->networkAddr, addr);
	mes->srcid = srcuid;
	
	return true; //TODO test implementation.
}
/**
* @brief      Initializes the message queue with pending friend messages.
*
* @return     true if successful, false otherwise.
*/
bool initPendingMessages() {
	return false; //TODO implement
}

/**
* @brief      Checks for permanent messages
*
* @return     true if successful, false otherwise.
*/
bool checkPermanentMessages() {
	return false; //TODO implement
}


/************************************************************************/
/* EXTERNAL METHODS                                                     */
/************************************************************************/

/**
* @brief      Initializes the message queue.
*
* @return     true if successful, false otherwise.
*/
bool initMessageQueue() {
	/**if (!initPermanentMessages()) {
	printe("Initializing permanent messages failed.\n");
	//TODO error
	}*/
	if (!initPendingMessages()) {
		printe("Initializing pending messages failed.\n");
		//TODO error
	}
	
	
	return false; //TODO implement
}

/**
* @brief      Adds a message to the queue.
*
* @param[in]  mes   The message to be added.
*
* @return     true if successful, false otherwise.
*/
bool addToQueue(Message *mes) {
	return false; //TODO implement
}

/**
* @brief      Removes message from queue.
*
* @param[in]  queuenumber   The number of the message to be removed.
*
* @return     true if successful, false otherwise.
*/
bool removeFromQueue(int queuenumber) {
	messageQueue[queuenumber].opcode = 0; //Set to invalid opcode.
	return false; //TODO implement
}

/**
* @brief      Check queue for a certain message.
*
* @param[in]  mes   The message, based on some of the params in the struct.
*
* @return     true if successful, false otherwise.
*/
bool checkQueue(Message *mes) {
	/* Parameters to compare: opcode, id, Network Address, port, */
	/* Checks queue for a message match. If match and not permanent, delete. */
	if (mes->opcode >= MAX_OPCODE) {
		printe("Opcode out of range.\n");
		return false;
	}
	
	return false; //TODO implement
}

/**
* @brief      Checks for old messages to be deleted.
*
* @return     Returns number of messages removed. Negative if problem occurs.
*/
bool flushOldMessages() {
	for (int i = 0; i < MAXQUEUESIZE; i++) {
		if (messageQueue[i].expiration >= queueTime) {
			removeFromQueue(i);
		}
	}
	return false; //TODO implement
}
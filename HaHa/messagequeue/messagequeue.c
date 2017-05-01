/*
* @file messagequeue.c
* @brief A queue of expected messages to be serviced implementation.
* @author Kevin Lee
* @date 4/8/2017 4:37:47 PM
*/

#include "messagequeue.h"
#include <string.h>

bool _compactQueue(Message * q);
uint8_t m_lastID;

/************************************************************************/
/* INTERNAL METHODS                                                     */
/************************************************************************/

Message messageQueue[MAXQUEUESIZE];

/* Helper */
/* Get Default Message Settings for each Opcode */
//WARNING INCOMPLETE
bool setSettingsByOpcode(Message *mes, opcode opcode) {
	uint32_t timeout = DEFMESSAGETIMEOUT;
	mes->opcode = opcode;
	switch(opcode) {
		//TODO THE VARIABLES ARE NOT SET CORRECTLY.
		//PING REQ
		case 1:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//HELP REQ
		case 2:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//HELP RESP
		case 3:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//HELP FROM ANYONE REQ
		case 4:
		timeout = timeout;
		mes->permanent = true;
		mes->broadcast = true;
		mes->numUses = 1;
		break;
		
		//HELP FROM ANYONE RESP
		case 5:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//FIND HOPS REQ
		case 6:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = true;
		mes->numUses = 1;
		break;
		
		//FIND HOPS RESP
		case 7:
		timeout = timeout;
		mes->permanent = true;
		mes->broadcast = false;
		mes->numUses = BROADCASTHOP;
		break;
		
		//FIND NEIGHBOR REQ
		case 8:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//FIND NEIGHBOR RESP
		case 9:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = BROADCASTHOP;
		break;
		
		//FRIEND REQ
		case 10:
		timeout = timeout;
		mes->permanent = false;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//FRIEND RESP
		case 11:
		timeout = timeout;
		mes->permanent = true;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		
		//UNFRIEND REQ
		case 12:
		timeout = timeout;
		mes->permanent = true;
		mes->broadcast = false;
		mes->numUses = 1;
		break;
		default:
		return false;
	}
	
	mes->expiration = queueTime;
	return true; //TODO Finish IMPLEMENT
}

/**
* @brief      Generates a message based on friend parameters.
*
* @param[in]  Friend	  Input friend
* @param[in]  permanent  The permanent tag to the message.
* @param      mes        A pointer to the message to be created.
*
* @return     true if successful, false otherwise, Message mes.
*/
bool generateFriendMessage(Friend *friend, Message *mes, opcode op) {
	//friend->id = 0;
	if (setSettingsByOpcode(mes, op)) {
		printe("Message has invalid opcode.\n");
		return false;
	}
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
bool generateMessage(Message *mes, opcode op, netaddr addr, uint16_t srcuid) {
	if (setSettingsByOpcode(mes, op)) {
		printe("Message has invalid opcode.\n");
		return false;
	}
	mes->id = FRIENDLISTSIZE; //Not a friend, out of boundary.
	if (strlen(addr) > MAXNETADDR) {
		printe("Network address too large.\n");
		return false;
	}
	strcpy(mes->srcAddr, addr);
	mes->srcid = srcuid;
	
	return true; //TODO test implementation.
}
/**
* @brief      Initializes the message queue with pending friend messages.
*
* @return     true if successful, false otherwise.
*/
bool initPendingMessages() {
	for (int i = 0; i < FRIENDLISTSIZE; i++) {
		//TODO pull data from storage system.
	}
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

bool _compactQueue(Message * q){
    //Compact down all entries of message queue
    //make change ids of entries to be increasing
    //update m_lastID to be next available id (also size of mqueue)
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
    m_lastID = 0;
	initPermanentMessages();
	initPendingMessages(); //rememeber set m_lastID if there are pending
	return false; //TODO implement
}

/**
 * @brief      Adds a message to the queue.
 *
 * @param[in]  mes   The message to be added.
 *
 * @return     true if successful, false otherwise.
 */
bool addToQueue(Message* mes) {
    if(m_lastID == MAXQUEUESIZE - 1)
        _compactQueue(messageQueue);
    mes->id = m_lastID;
    messageQueue[m_lastID] = *mes;
    
    m_lastID = (m_lastID + 1) % MAXQUEUESIZE; //failsafe
    printf("ADDED MESSAGE TO Q:%d-%d\n",mes->id, mes->opcode );
    return true;
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
* @return     Number of opcodes present, or negative if failure.
*/
int checkQueue(opcode op, Network *net, Event eventList[]) {
	/* Parameters to compare: opcode, id, Network Address, port, */
	/* Checks queue for a message match. If match and not permanent, delete. */
	int numOpcodesPresent = 0;
	
	if (op > MAX_OPCODE || op == 0) {
		printe("Opcode out of range.\n");
		return -1;
	}
	
	//Request messages of the broadcast variety and
	if (op == HELP_FROM_ANYONE_REQUEST || op == FIND_HOPS_REQUEST
	|| op == FIND_NEIGHBORS_REQUEST || op == FRIEND_REQUEST
	|| op == UNFRIEND_REQUEST) {
		//Permanent message gets automatically handled.
		//TODO check for errors.
		return 1;
	}
	int j = 0;
	for (int i = 0; i < MAXQUEUESIZE; i++) {
		if (messageQueue[i].opcode == op) {
			numOpcodesPresent++;
			eventList[j].srcid = messageQueue[i].srcid;
			strcpy(eventList[j].srcAddr, messageQueue[i].srcAddr); //TODO check bounds
			j++;
		}
	}

	return numOpcodesPresent; //TODO verify correctness
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
	return false; //TODO verify this works.
}
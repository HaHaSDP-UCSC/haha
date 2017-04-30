/*
 * @file messagequeue.c
 * @brief A queue of expected messages to be serviced implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#include "messagequeue.h"

bool _compactQueue(Message * q);
uint8_t m_lastID;

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
bool generateMessage(Friend friend, bool permanent, Message *mes) {
     //Check if m_lastID is about to rollover, if so compact message queue
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
 * @brief      Initializes permanent messages into the queue.
 *
 * @return     true if successful, false otherwise.
 */
bool initPermanentMessages() {
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
	return false; //TODO implement
}

/**
 * @brief      Check queue for a certain message.
 *
 * @param[in]  mes   The message, based on some of the params in the struct.
 *
 * @return     true if successful, false otherwise.
 */
bool checkQueue(Message* mes) {
	/* Parameters to compare: opcode, id, Network Address, port, */
	/*Checks queue for a message match. If match and not permanent, delete.*/
	return false; //TODO implement
}

/**
 * @brief      Checks for old messages to be deleted.
 *
 * @return     Returns number of messages removed. Negative if problem occurs.
 */
bool flushOldMessages() {
	return false; //TODO implement
}

bool initMessage(Message * m){
   
}
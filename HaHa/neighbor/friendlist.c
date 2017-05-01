/*
 * @file friendlist.c
 * @brief List of friends with accompanying data implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 
#include "friendlist.h"
#include "messagequeue/messagequeue.h"

/************************************************************************/
/* INTERNAL METHODS                                                     */
/************************************************************************/

/**
 * @brief      Writes the friend list to storage.
 *
 * @return     true if successful, false otherwise.
 */
bool writeToFriendListStorage() {
	return false; //TODO Implement
}

/**
 * @brief      Reads the friend list from storage.
 *
 * @return     true if successful, false otherwise.
 */
bool readFromFriendListStorage() {
	return false; //TODO Implement
}

/************************************************************************/
/* EXTERNAL METHODS                                                     */
/************************************************************************/

/**
 * @brief      Initializes the friend list.
 *
 * @return     true if successful, false otherwise.
 */
bool initFriendList() {
	return false; //TODO Implement
}

/**
 * @brief      Adds a friend.
 *
 * @param[in]  friend	The friend to be added to the list.
 * @return     true if successful, false otherwise.
 */
bool addFriend(Friend *f) {
	//if(friendList[priority] == 0)
        ////reogranize list
        //printf("Reorganize list");
    //else
        friendList[f->priority] = *f;
    Friend* ftemp = &friendList[f->priority];
    printf("Added friend: %s", ftemp->firstname);
    printf("netaddr:[");
    printBuff(f->networkaddr, 8, "%c");
    printf("]\n");
    return true;
}

/**
 * @brief      Removes a friend from the list, and updates the list.
 *
 * @param[in]  net	The network parameters of friend.
 *
 * @return     true if successful, false otherwise.
 */
bool removeFriend(Network* net) {
	return false; //TODO Implement	
}

/**
 * @brief      Checks for a friend in the list.
 * 
 * @param[in]  net	The network parameters of the friend.
 *
 * @return     true if successful, false otherwise.
 */
bool checkForFriend(Network* net) {
	return false; //TODO Implement	
}

void addTestFriend(char *fname, char*lname, char* addr){
    Friend *f = malloc(sizeof(Friend));
    strcpy(f->firstname, fname);
    strcpy(f->lastname, lname);
    f->id = 1;
    //printf("%s", addr);
    //printBuff(addr, 8, "%c");
    //printBuff(convert_asciihex_to_byte(addr), 8, "%c");
    uint8_t* t = convert_asciihex_to_byte(addr);
    memcpy(f->networkaddr,t, 8);
    printBuff(f->networkaddr, 8, "%c");
    printBuff(t, 8, "%c");
    //strcpy(f->networkaddr,addr);
    f->port = 0x1;
    f->priority = 0x1;
    addFriend(f);
}


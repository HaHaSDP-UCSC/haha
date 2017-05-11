/*
 * @file friendlist.c
 * @brief List of friends with accompanying data implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 
#include "friendlist.h"
#include "messagequeue/messagequeue.h"

uint8_t numLocal = 0;

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
    printd("Added friend: %s %s", ftemp->firstname, ftemp->lastname);
    printd("netaddr:[");
    printBuff(f->networkaddr, 8, "%c");
    printd("]\n");
    return true;
}

/* Move friend to different priority, and modify friend parameters */
bool modifyFriend(Friend *f, int moveFriend) {
	if (moveFriend >= 0 && moveFriend < FRIENDLISTSIZE) {
		
	}
	return false; //TODO Implement
}


/**
 * @brief      Removes a friend from the list, and updates the list.
 *
 * @param[in]  net	The network parameters of friend.
 *
 * @return     true if successful, false otherwise.
 */
bool removeFriend(Network* net) {
	//TODO mark friend as null.
	
	//Friend list reshuffle.
	//O(n^2) time but only a few items so its okay.
	//TODO code untested
	for (int i = 0; i < FRIENDLISTSIZE; i++) {
		if (!strcmp(friendList[i].networkaddr, net->src) 
		&& !strcmp(friendList[i].port, net->id)) {
			friendList[i].id == NULL;
			for (int j = i; i < FRIENDLISTSIZE-i-1; j++) {
				friendList[j] = friendList[j+1];
			}
		}
	}
	return true;
}

/**
 * @brief      Checks for a friend in the list.
 * 
 * @param[in]  net	The network parameters of the friend.
 *
 * @return     true if successful, false otherwise.
 */
int checkForFriend(Network* net) {
    printd("Searching for friend with addr:");
    printBuff(net->src, 8, "%x");
    printd("\n");
    for(int i=0; i<FRIENDLISTSIZE; ++i){
        printd("searching entry %d", i);
        if(netCompare(friendList[i].networkaddr, net->src))
	        return i;
    }            
    return -1;
}

/* Test Friend */
void addTestFriend(char *fname, char*lname, char* addr){
    Friend *f = malloc(sizeof(Friend));
    strcpy(f->firstname, fname);
    strcpy(f->lastname, lname);
    f->id = 1;
    uint8_t* t = convert_asciihex_to_byte(addr);
    memcpy(f->networkaddr,t, 8);
    printd("Adding test friend\n");
    printBuff(f->networkaddr, 8, "%c");
    printBuff(t, 8, "%c");
    //strcpy(f->networkaddr,addr);
    f->port = 0x1;
    f->priority = 0x1;
    addFriend(f);
}

/* Test User */
void addTestLocalUser(char *fname, char*lname, uint16_t port){
    Friend *f = malloc(sizeof(Friend));
    strcpy(f->firstname, fname);
    strcpy(f->lastname, lname);
    f->port = port;
    printd("Adding test Local User\n");
    Friend* ftemp = &localUsers[numLocal++];
}
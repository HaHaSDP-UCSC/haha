/*
 * @file friendlist.c
 * @brief List of friends with accompanying data implementation.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 
#include "friendlist.h"
#include "messagequeue/messagequeue.h"

#include <string.h>
#include <malloc.h>

uint8_t numLocal = 0;
uint8_t numFriends = 0;
LocalUser localUsers[MAXLOCAL];

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
    friendList[numFriends] = *f;
    Friend* ftemp = &friendList[numFriends];
    printd("Added friend: %s %s", ftemp->firstname, ftemp->lastname);
    printd("netaddr:[");
    printBuff(f->networkaddr, 8, "%c");
    printd("]\n");
    numFriends++;
    return true;
}

bool addLocalUser(LocalUser *f) {
	localUsers[numLocal] = *f;
	LocalUser* ftemp = &localUsers[numLocal];
	printd("Added local user: %s %s", ftemp->friend.firstname, ftemp->friend.lastname);
	printd("netaddr:[");
	printBuff(f->friend.networkaddr, 8, "%c");
	printd("]\n");
	numLocal++;
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
bool removeFriend(Friend *f) {
	//TODO mark friend as null.
	if (numFriends <= 0) {
		return false; //No friends.
	}
	//Friend list reshuffle.
	//O(n^2) time but only a few items so its okay.
	//TODO code untested
	for (int i = 0; i < FRIENDLISTSIZE; i++) {
		if (!strcmp(friendList[i].networkaddr, f->networkaddr) 
		&& friendList[i].port == f->port) {
			numFriends--;
			friendList[i].id = 0; //Null id.
			int priority = friendList[i].priority;
			//Reshuffle friends up.
			for (int j = i; j < FRIENDLISTSIZE-i-1; j++) {
				friendList[j] = friendList[j+1];
			}
			//Fix priorities.
			for (int j = 0; j < numFriends; j++) {
				if (friendList[j].priority > priority) {
					friendList[j].priority--; //TEST CODE
				}
			}

			break;
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
Friend * checkForFriend(Network* net) {
    printd("Searching for friend with addr:");
    printBuff(net->src, 8, "%x");
    printd("\n");
    for(int i=0; i<FRIENDLISTSIZE; ++i){
        printd("searching entry %d", i);
        if(netCompare(friendList[i].networkaddr, &net->src))
	        return &friendList[i];
    }            
    return NULL;
}

/* Test Friend */
void addTestFriend(char *fname, char*lname, char* addr){
    Friend *f = malloc(sizeof(Friend));
    strcpy(f->firstname, fname);
    strcpy(f->lastname, lname);
    f->id = numFriends;
    uint8_t* t = (uint8_t *) convert_asciihex_to_byte(addr);
    memcpy(f->networkaddr,t, 8);
    printd("Adding test friend\n");
    printBuff(f->networkaddr, 8, "%c");
    printBuff(t, 8, "%c");
    //strcpy(f->networkaddr,addr);
    f->port = 0x1;
    f->priority = numFriends;
    addFriend(f);
	numFriends++;
}

/* Test User */
void addTestLocalUser(char *fname, char*lname, uint16_t port){
	//Friend *f = malloc(sizeof(Friend));
	//Local User Parameters
	printd("Adding test Local User\n");
	localUsers[numLocal].friend.port = port;
	strcpy(localUsers[numLocal].homeaddr, "HOME ADDR GOES HERE"); //TODO Set this.
	strcpy(localUsers[numLocal].phoneaddr, "808-909-9999"); //TODO Set this.
	//Required Friend Parameters
	strcpy(localUsers[numLocal].friend.firstname, fname);
	strcpy(localUsers[numLocal].friend.lastname, lname);
	strcpy(localUsers[numLocal].friend.networkaddr, "NETADDR"); //TODO set this.
    //strcpy(f->firstname, fname);
    //strcpy(f->lastname, lname);
    //f->port = port;
    //Friend* ftemp = &localUsers[numLocal++].friend;
	numLocal++;
}
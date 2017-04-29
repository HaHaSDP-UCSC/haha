/*
 * @file friendlist.h
 * @brief List of friends with accompanying data.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

#include "utils/hahaUtils.h"
#include "network/network.h"
#include "network/packet.h"

//Not to be larger than uint8_t
#define FRIENDLISTSIZE 10 //TODO make this an option and smaller.

typedef struct {
	//uint8_t id; //Internal ID. Ties into the friendlist.
	uint8_t priority; //What level they are on the friend list.
	char firstname[MAXFIRSTNAME]; //First Name
	char lastname[MAXLASTNAME]; //Last Name
	unsigned char networkaddr[MAXNETADDR]; //Network Address.
	uint16_t port; //Network port.
	uint32_t lastresponse; //32-bit offset from boot timer.
	uint16_t responseflag; //16 Available flags for expected responses. //TODO includes if registered friend.
} Friend;

Friend friendList[FRIENDLISTSIZE];

//TODO @kevin @brian storage for FriendList.
bool initFriendList();
bool writeToFriendListStorage(); //TODO INTERNAL REMOVE
bool readFromFriendListStorage(); //TODO INTERNAL REMOVE
bool addFriend();
bool removeFriend(Network* net); //Remove based on srcuid and netaddr.
bool checkForFriend(Network* net);

#endif /* FRIENDLIST_H_ */
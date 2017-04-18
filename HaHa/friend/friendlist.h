/*
 * @file friendlist.h
 * @brief List of friends with accompanying data.
 * @author Kevin Lee
 * @date 4/8/2017 4:37:47 PM
 */ 

#ifndef FRIENDLIST_H_
#define FRIENDLIST_H_

#include "utils/hahaUtils.h"
#include "messagequeue/packet.h"
#include "network/network.h"

#define FRIENDLISTSIZE 10

typedef struct {
	BYTE id; //Internal ID. 
	BYTE priority;
	char firstname[MAXFIRSTNAME];
	char lastname[MAXLASTNAME];
	char networkaddr[MAXNETADDR]; //TODO Finalize data structure.
	uint16_t port; //Network port.
	long lastresponse; //TODO time data structure
	uint16_t responseflag; //16 Available flags for expected responses.
} Friend;

Friend friendList[FRIENDLISTSIZE];

//TODO @kevin @brian storage for FriendList.
bool initFriendList();
bool writeToFriendListStorage();
bool readFromFriendListStorage();
bool addFriend();
bool removeFriend(Network net, Packet packet); //Remove based on srcuid and netaddr.

#endif /* FRIENDLIST_H_ */
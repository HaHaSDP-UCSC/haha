/*
 * @file packet.h
 * @brief Packet creation and handling.
 * @author Kevin Lee
 * @date 4/14/2017
 */ 

#ifndef PACKET_H_
#define PACKET_H_

typedef enum {
	PING_REQUEST = 0x01,
	HELP_REQUEST,
	HELP_RESPONSE,
	HELP_FROM_ANYONE_REQUEST,
	HELP_FROM_ANYONE_RESPONSE,
	FIND_HOPS_REQUEST,
	FIND_HOPS_RESPONSE,
	FIND_NEIGHBORS_REQUEST,
	FIND_NEIGHBORS_RESPONSE,
	FRIEND_REQUEST,
	FRIEND_RESPONSE,
	UNFRIEND_REQUEST
} op;

typedef unsigned char opcode;
typedef unsigned char ttl;
typedef uint16_t uid;

typedef struct {
	//Packet params here
	//opcode opcode;
	opcode opcode;
	flags flags;
	uid SRCUID;
	uid DESTUID;
	uid ORIGINUID;
	char SRCFIRSTNAME[MAXFIRSTNAME]; //TODO optimize for space.
	char SRCLASTNAME[MAXFIRSTNAME]; //TODO optimize for space.
	char SRCPHONE[MAXPHONE]; //TODO optimize for space.
	char SRCHOMEADDR[MAXHOMEADDR]; //TODO optimize for space.
	ttl ttl;
} Packet;

int pingHandler(Packet p);

int helpRequestHandler(Packet p);
int helpResponseHandler(Packet p);
int helpFromAnyoneRequestHandler(Packet p);
int helpFromAnyoneResponseHandler(Packet p);

int findHopsRequestHandler(Packet p);
int findHopsResponseHandler(Packet p);

int findNeighborsRequestHandler(Packet p);
int findNeighborsResponseHandler(Packet p);

int friendRequestHandler(Packet p);
int friendResponseHandler(Packet p);
int unfriendRequestHandler(Packet p);

#endif /* PACKET_H_ */
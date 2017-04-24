/*
 * @file packet.h
 * @brief Application Packet creation and handling.
 * @author Kevin Lee
 * @date 4/14/2017
 */ 

#ifndef PACKET_H_
#define PACKET_H_

#include "flags.h"
#include "utils/hahaUtils.h"

//typedef char *opcode;
typedef char opcode;
typedef uint8_t ttl;
typedef uint16_t uid;

#define MAXFIRSTNAME 17
#define MAXLASTNAME 17
#define MAXPHONE 2
#define MAXHOMEADDR 2
#define MAXNETADDR 4 //4 Bytes (32-bits)
#define MAXPORT 2

#define START_OPCODE 0x01

typedef enum {
	PING_REQUEST = START_OPCODE,
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

typedef struct {
	//Packet params here
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


/* Handling received packets */
//Moved to network.h 

/* Handles sending packets */
//TODO add.

#endif /* PACKET_H_ */
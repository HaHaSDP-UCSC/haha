/* Definitions of network types */
#ifndef _HA_NETDEFS
#define _HA_NETDEFS

//#include "devtools.h"
#include "flags.h"

/* BASE COMMUNICATION */
typedef char* opcode;

/* Application Packets */
typedef struct {
	//Packet params here
	opcode opcode;
	flags flags;
	char* src;
	char* dst;
	char* data;
} Packet;

#endif

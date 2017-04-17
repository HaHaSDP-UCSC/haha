///* Definitions of network types */
//#ifndef _HA_NETDEFS
//#define _HA_NETDEFS
//
////#include "devtools.h"
//#include "flags.h"
//
///* BASE COMMUNICATION */
//typedef char* opcode;
//
///* Application Packets */
//typedef struct {
	////Packet params here
	//opcode opcode;
	//flags flags;
	//char* src;
	//char* dst;
	//char* data;
//} Packet;
//
//#endif

//TODO @brian, new network.h
/* Definition of different network layers. Network data should be passed up
 * through this.
 */

#ifndef _HA_NETWORK
#define _HA_NETWORK

#include "utils/hahaUtils.h"
#include "messagequeue/packet.h"
#define netaddr char *//TODO @brian define better to comply with xbee
#define MAX_NET_ARRAY 10
typedef struct {
	//TODO @brian Network information to send/receive packet. May need more.
	opcode opcode;
	netaddr src;
	netaddr dest;
	bool broadcast;
	BYTE ttl;
} Network;

unsigned int netIDLast;
Network NET_ARRAY[MAX_NET_ARRAY];

bool networkStatus(); //TODO flesh out.

/* Function that will be registered with net drive as callback */
void app_packet_handler(char* data, uint8_t id);

/* Op Code Handlers */
void ping_request_handler();
void help_request_handler();
void help_response_handler();
void help_request_anyone_handler();
void help_response_anyone_handler();
void find_hops_request_handler();
void find_hops_response_handler();
void find_neighbors_request_handler();
void find_neighbors_response_handler();
void friend_request_handler();
void friend_response_handler();
void unfriend_request_hanlder();


#endif /* NETWORK_H_ */
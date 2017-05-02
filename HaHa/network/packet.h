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
#include "network.h"
#include "neighbor/friendlist.h"

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
} Op;

typedef struct {
	//Packet params here
    uint8_t id;
	opcode opcode;
	flags flags;
	uid SRCUID;
	uid DESTUID;
	uid ORIGINUID;
	//char SRCFIRSTNAME[MAXFIRSTNAME];
	//char SRCLASTNAME[MAXFIRSTNAME];
	//char SRCPHONE[MAXPHONE];
	//char SRCHOMEADDR[MAXHOMEADDR];
    char *SRCFIRSTNAME; //These already exist in other structures, just need pointers
    char *SRCLASTNAME;
    char *SRCPHONE;
    char *SRCHOMEADDR;
	ttl ttl;
} Packet;

/* Handling received packets */
//void app_packet_handler(char* data, uint16_t len, uint8_t* src, uint8_t id);
void app_packet_handler(Network* info);

/* Op Code Handlers */
typedef void (*opcode_handler_fn_t)(Packet *p);
//typedef void (*opcode_parser_fn_t)(char *data, uint16_t len, uint64_t src, uint8_t id);
opcode_handler_fn_t haha_packet_handlers[12];
//opcode_parser_fn_t haha_packet_parsers[12];
/**
 * @brief      { Register opcode functions }
 *
 * @param[in]  t     { Function to call for opcode }
 */
void register_opcode_handler_function(opcode_handler_fn_t t, Op opcode);
//Call from network_init()
void opcodes_init();

/* Handle the packets */
void ping_request_handler(Packet *p);
void help_request_handler(Packet *p);
void help_response_handler(Packet *p);
void help_request_anyone_handler(Packet *p);
void help_response_anyone_handler(Packet *p);
void find_hops_request_handler(Packet *p);
void find_hops_response_handler(Packet *p);
void find_neighbors_request_handler(Packet *p);
void find_neighbors_response_handler(Packet *p);
void friend_request_handler(Packet *p);
void friend_response_handler(Packet *p);
void unfriend_request_handler(Packet *p);

/* Handles sending packets */

void send_ping_request(Friend *f);

#endif /* PACKET_H_ */
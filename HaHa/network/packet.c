/*
 * packet.c
 *
 * Created: 4/23/2017 7:10:04 PM
 *  Author: Brian
 */ 

#include "packet.h"
#include "network.h"
#include "flags.h"
#include "messagequeue/messagequeue.h"
#include "basecomm.h"
#include <stdlib.h>
 
void opcodes_init(){
    haha_packet_handlers[PING_REQUEST] = ping_request_handler;
	//haha_packet_handlers[HELP_REQUEST] = help_request_handler;
	//haha_packet_handlers[HELP_RESPONSE] = help_response_handler;
	//haha_packet_handlers[HELP_FROM_ANYONE_REQUEST] = help_request_anyone_handler;
	//haha_packet_handlers[HELP_FROM_ANYONE_RESPONSE] = help_response_anyone_handler;
	//haha_packet_handlers[FIND_HOPS_REQUEST] = find_hops_request_handler;
	//haha_packet_handlers[FIND_HOPS_RESPONSE] = find_hops_response_handler;
	//haha_packet_handlers[FIND_NEIGHBORS_REQUEST] = find_neighbors_request_handler;
	//haha_packet_handlers[FIND_NEIGHBORS_RESPONSE] = find_neighbors_response_handler;
	//haha_packet_handlers[FRIEND_REQUEST] = friend_request_handler;
	//haha_packet_handlers[FRIEND_RESPONSE] = friend_response_handler;
	//haha_packet_handlers[UNFRIEND_REQUEST] = unfriend_request_handler;

}    

//void app_packet_handler(char* data, uint16_t len, uint8_t* src, uint8_t id){
void app_packet_handler(Network *info){
    HAHADEBUG("In app packet handler\n");
    uint8_t opcode = info->data[0];
    HAHADEBUG("Found opcode: 0x%x\n", opcode);
    printf("Netstuff:");
    printBuff(info->src, 8, "%c");
    //For packets that need a corresponding message queue entry, check and then 
    //drop if there isn't a corresponding event.
    //if (!isExpecting(opcode, PING_REQUEST_FLAG)) {
        //*  //drop packet
    //}
    
    //Send to parser
    Packet *p = malloc(sizeof(Packet));
    assert(p != NULL);
    int success = convertFromDataToPacket(p, info->data, info->len);
    if(!success){
        HAHADEBUG("Error in packet.");
        free(p);
        return;
    }        
    p->id = info->id;
    (haha_packet_handlers[p->opcode])(p);
    netArrayAdd(info);
} 

void register_opcode_handler_function(opcode_handler_fn_t t, Op opcode){
    haha_packet_handlers[opcode] = t;
}

void ping_request_handler(Packet *p){
    if(IS_ACK(p->flags)){
        printf("IS Ping Ack");
    }
    else{
        printf("Is Ping Request");
        //Ping request network should still be in queue
        int i = netArrayReturn(p->id);
        if(i == NOT_FOUND){
             HAHADEBUG("net item not found");
             return;
        }             
        Network* net = &NET_ARRAY[i];
        netaddr t = net->dest;
        net->dest = net->src;
        net->src = t;
        sendPacket(p, net);
    }        
}   
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
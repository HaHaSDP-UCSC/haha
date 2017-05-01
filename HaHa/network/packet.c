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
#include "neighbor/friendlist.h"
#include <stdlib.h>
#include <string.h>
 
void opcodes_init(){
    haha_packet_handlers[PING_REQUEST] = ping_request_handler;
	haha_packet_handlers[HELP_REQUEST] = help_request_handler;
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

void printNetAddr(netaddr *t){
    printf("NetAddr:[");
    printBuff(t, 8, "%c");
    printf("]\n");
}

//void app_packet_handler(char* data, uint16_t len, uint8_t* src, uint8_t id){
void app_packet_handler(Network *info){
    HAHADEBUG("In app packet handler\n");
    uint8_t opcode = info->data[0];
    HAHADEBUG("Found opcode: 0x%x\n", opcode);
    printf("Netsrc:");
    printBuff(info->src, 8, "%x");
    printf("\n");
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
    netArrayAdd(info);
    (haha_packet_handlers[p->opcode])(p);
} 

void register_opcode_handler_function(opcode_handler_fn_t t, Op opcode){
    haha_packet_handlers[opcode] = t;
}   

void copy_friend_to_packet(Friend *f, Packet* p){
    p->DESTUID = f->port;
    strcpy(p->SRCFIRSTNAME, f->firstname);
    strcpy(p->SRCLASTNAME, f->lastname);
}

void send_ping_request(Friend *f){
    Network* n = malloc(sizeof(Network));
    Packet* p = malloc(sizeof(Packet));
    //printf("PACKET:");
    //printBuff(f->networkaddr, 8, "%c");
    //memcpy(n->dest, f->networkaddr, 8);
    n->dest = f->networkaddr;
    printNetAddr(n->dest);
    copy_friend_to_packet(f, p);
    p->opcode = 0x1;
    p->DESTUID = 0x1;
    CLR_FLAGS(p->flags);
    sendPacket(p, n);
    //Add a corresponding message
    Message *m = malloc(sizeof(Message));
    memcpy(m->networkAddr, n->dest, 8);
    m->opcode = PING_REQUEST;
    m->srcid = 0x1; //TODO: somehow track current user
    addToQueue(m);
}

void ping_request_handler(Packet *p){
    printf("in ping handler\n");
    if(IS_ACK(p->flags)){
        printf("IS Ping ACK!\n");
    }
    else{
        printf("Is Ping Request\n");
        //Ping request network should still be in queue
        int i = netArrayReturn(p->id);
        if(i == NOT_FOUND){
             HAHADEBUG("net item not found");
             return;
        }             
        Network* net = &NET_ARRAY[i];
        //Packet* p2 = malloc(sizeof(Packet));
        Packet p2;
        p2.opcode = PING_REQUEST;
        CLR_FLAGS(p2.flags);
        SET_ACK(p2.flags);
        net->dest = net->src;
        //net->src = myNetID;
        p2.SRCUID = 0x1;
        strcpy(p2.SRCFIRSTNAME, "Brian");
        strcpy(p2.SRCLASTNAME, "Nichols");
        printNetAddr(net->dest);
        sendPacket(&p2, net);
        free(p);
    }        
}   

void _send_help_request(netaddr t){
    
}

void send_help_request(Friend f){
    
}
void help_request_handler(Packet *p){
    
}
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
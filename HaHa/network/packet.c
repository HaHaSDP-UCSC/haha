/*
 * packet.c
 *
 * Created: 4/23/2017 7:10:04 PM
 *  Author: Brian
 */ 

#include "packet.h"
#include "network.h"
 
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

void app_packet_handler(char* data, uint16_t len, uint8_t* src, uint8_t id){
    HAHADEBUG("In app packet handler\n");
    //Parse opcode and send to handler
    //char ct[3];
    //char* t;
    //ct[0] = data[0];
    //ct[1] = data[1];
    //ct[2] = '\0';
    uint8_t opcode = data[0];
    HAHADEBUG("Found opcode: 0x%x\n", opcode);
    //int source = 0x0013A200414F50ea;
    //HAHADEBUG("Source: 0x%x%x",source>>32, source);
    //HAHADEBUG("Source: %d 0x%x%x",0,source>>32, source);
    
    HAHADEBUG("Source: %d 0x%x",0,src, src);
    //HAHADEBUG("%d %x\n%d\n",0, src);
    char dst[10];
     //dst[0] = (src >> 56) & 0xFF;
     //dst[1] = (src >> 48) & 0xFF;
     //dst[2] = (src >> 40) & 0xFF;
     //dst[3] = (src >> 32) & 0xFF;
     //dst[4] = (src >> 24) & 0xFF;
     //dst[5] = (src >> 16) & 0xFF;
     //dst[6] = (src >> 8) & 0xFF;
     //dst[7] = src& 0xFF;
          //dst[0] = (src >> 0) & 0xFF;
          //dst[1] = (src >> 8) & 0xFF;
          //dst[2] = (src >> 16) & 0xFF;
          //dst[3] = (src >> 24) & 0xFF;
          //dst[4] = (src >> 32) & 0xFF;
          //dst[5] = (src >> 48) & 0xFF;
          //dst[6] = (src >> 56) & 0xFF;
          //dst[7] = src& 0xFF;
     //printBuff(dst, 8, "0x%x ");
    haha_packet_parsers[opcode](data, len, src, id);
} 

void register_opcode_handler_function(opcode_handler_fn_t t, Op opcode){
    haha_packet_handlers[opcode] = t;
}

void register_opcode_parser_function(opcode_parser_fn_t t, Op opcode){
    haha_packet_parsers[opcode] = t;
}

void ping_request_handler(Packet *p){
       
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
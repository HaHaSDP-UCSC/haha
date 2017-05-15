/*
 * packet.c
 *
 * Created: 4/23/2017 7:10:04 PM
 *  Author: Brian, Kevin
 */ 

#include "packet.h"
#include "network.h"
#include "flags.h"
#include "messagequeue/messagequeue.h"
#include "basecomm.h"
#include "neighbor/friendlist.h"
#include <stdlib.h>
#include <string.h>
 
void opcodes_init() {
    haha_packet_handlers[PING_REQUEST] = ping_request_handler;
	haha_packet_handlers[HELP_REQUEST] = help_request_handler;
	haha_packet_handlers[HELP_RESPONSE] = help_response_handler;
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

void printNetAddr(netaddr *t) {
    printf("NetAddr:[");
    printBuff(t, 8, "%c");
    printf("]\n");
}

//void app_packet_handler(char* data, uint16_t len, uint8_t* src, uint8_t id){
void app_packet_handler(Network *info) {
    HAHADEBUG("In app packet handler\n");
    opcode opcode = info->data[0];
    HAHADEBUG("Found opcode: 0x%x\n", opcode);
    flags flag = info->data[1];
    HAHADEBUG("Found flags: 0x%x\n", flag);
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
    //assert(p != NULL);
	if (p == NULL) {
		printe("Packet Malloc Error.");
		return;
	}
    int success = convertFromDataToPacket(p, info->data, info->len);
    if(!success){
        HAHADEBUG("Error in packet.");
        free(p);
        return;
    }        
    p->id = info->id;
    netArrayAdd(info);
    (haha_packet_handlers[p->opcode])(p);
	//TODO free(p); //Is this the end of the packet?
} 

void register_opcode_handler_function(opcode_handler_fn_t t, Op opcode) {
    haha_packet_handlers[opcode] = t;
}   
/* Given a Friend, Local User, and Packet, with packet having opcode and flags
	already pre-registered. Friend is another user base station user. */
void copy_friend_to_packet(Friend *f, LocalUser *self, Packet* p) {
	
	//TODO set up to work for each case. (switch case)
	//TODO or we can be lazy and copy everything to the packet,
	//and have the packet sender handle it.
	
	//Lazy Method
	//p->opcode = opcode; //Should already be passed in.
	//p->flags = flag; //Should already be passed in.
	p->SRCUID = self->friend.port;
	p->DESTUID = f->port;
	//p->ORIGINUID == u->friend.port; //These are not used in friends packets.
	strcpy(p->SRCFIRSTNAME, self->friend.firstname);
	strcpy(p->SRCLASTNAME, self->friend.lastname);
	strcpy(p->SRCHOMEADDR, self->homeaddr);
	strcpy(p->SRCPHONE, self->phoneaddr);
	p->ttl = 0; //TODO make this a proper number.
	p->id = 0; //TODO set this to the id number in the packet storage table.
}

void send_ping_request(Friend *f) {
    Network* n = malloc(sizeof(Network));
    Packet* p = malloc(sizeof(Packet));
	LocalUser *self = &localUsers[0]; //TODO make this scalable.
	
    //printf("PACKET:");
    //printBuff(f->networkaddr, 8, "%c");
    //memcpy(n->dest, f->networkaddr, 8);
    n->dest = f->networkaddr;
    printNetAddr(n->dest);
	
    p->opcode = PING_REQUEST;
	CLR_FLAGS(p->flags);
    copy_friend_to_packet(f, self, p);
    sendPacket(p, n);
	
    //Add a corresponding message
    Message *m = malloc(sizeof(Message));
    //setSettingsByOpcode(m, PING_REQUEST);
    generateFriendMessage(f, m, PING_REQUEST);
    //memcpy(m->srcAddr, n->dest, 8);
    addToQueue(m);
}

/* Helper Function */
/* Returns network info from id attached to packet.*/
bool getNetInfo(Packet *p, Network *net) {
	int i = netArrayReturn(p->id);
	if(i == NOT_FOUND){
		return false;
	}
	net = &NET_ARRAY[i];
	return true;
}

void ping_request_handler(Packet *p){
    printf("in ping handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
    if(IS_ACK(p->flags)){
        printf("IS Ping ACK!\n");
        //Lookup message/event
        Event *e = malloc(sizeof(Event) * MAXQUEUESIZE);
        int numEntries = checkQueue(PING_REQUEST, p->flags, net, e);
        if(numEntries){
            printd("Found %d message for opcode %d\n", numEntries, p->opcode);
            printNetAddr(net->src);
            //update friendlist last response
            uint8_t index = checkForFriend(net);
            friendList[index].lastresponse = queueTime;
        }
        removeFromQueueEvents(e, numEntries);
        free(e);
    }
    else{
        printf("Is Ping Request\n");
        //Ping request network should still be in queue
        int i = netArrayReturn(p->id);
        if(i == NOT_FOUND){
             HAHADEBUG("net item not found");
             return;
        }             
        net = &NET_ARRAY[i];
        //Packet* p2 = malloc(sizeof(Packet));
		
        net->dest = net->src;
        
		//TODO look up friend.
		Friend *f = &friendList[0]; //TODO set this properly.
		LocalUser *self = &localUsers[0]; //TODO set this properly.
        Packet p2;
        p2.opcode = PING_REQUEST;
        CLR_FLAGS(p2.flags);
        SET_ACK(p2.flags);
		copy_friend_to_packet(f, self, &p2);
		
        /**
		p2.SRCUID = 0x1; //TODO fix
        char *fname = malloc(sizeof("Brian"));
        char *lname = malloc(sizeof("Nichols"));
        strcpy(fname, "Brian");
        strcpy(lname, "Nichols");
        p2.SRCFIRSTNAME = fname;
        p2.SRCLASTNAME = lname;
        //strcpy(p2.SRCFIRSTNAME, fname);
        //strcpy(p2.SRCLASTNAME, "Nichols");
        printNetAddr(net->dest);
		*/
		
        sendPacket(&p2, net);
        free(p);
    }        
}   

/**
void _send_help_request(netaddr *t){
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	copy_friend_to_packet(f, p);
	p->opcode = HELP_REQUEST;
	p->DESTUID = 0x1; //TODO fix
	CLR_FLAGS(p->flags);
	sendPacket(p, n);
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	//setSettingsByOpcode(m, PING_REQUEST);
	generateFriendMessage(f, m, HELP_REQUEST);
	//memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
}
*/

void send_help_request(Friend *f, LocalUser *self){
    Network* n = malloc(sizeof(Network));
    Packet* p = malloc(sizeof(Packet));
    //printf("PACKET:");
    //printBuff(f->networkaddr, 8, "%c");
    //memcpy(n->dest, f->networkaddr, 8);
	
    n->dest = f->networkaddr;
    printNetAddr(n->dest);
	
    p->opcode = HELP_REQUEST;
    CLR_FLAGS(p->flags);
    copy_friend_to_packet(f, self, p);
    sendPacket(p, n);
	
    //Add a corresponding message
    Message *m = malloc(sizeof(Message));
    setSettingsByOpcode(m, HELP_REQUEST);
    memcpy(m->srcAddr, n->dest, 8);
    addToQueue(m);
    free(n);
    free(p);
}

void send_help_request_ack(Friend *f, LocalUser *self) {
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	//printf("PACKET:");
	//printBuff(f->networkaddr, 8, "%c");
	//memcpy(n->dest, f->networkaddr, 8);
	
	n->src = self->friend.networkaddr;
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = HELP_REQUEST;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Add a corresponding message
	/**
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, HELP_REQUEST);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	*/
	free(n);
	free(p);
}

void help_request_handler(Packet *p) {
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printv("FRIEND_REQ_HANDLER ACK\n");
		//Display to user that device was able to connect, pending response.
		//Add message/event for a HELP_RESPONSE ACK (Only after user accepts) //TODO NOT HERE.
	} else {
		printv("FRIEND_REQ_HANDLER\n");
		//Check if friend
		for (int i = 0; i < FRIENDLISTSIZE; i++) {
			int isFriend = checkForFriend(net);
			if (isFriend != -1) {
				//Send HELP_REQUEST_ACK
				send_help_request_ack(f, localUsers[0]); //TODO not zero
				//Display to user that friend is in need.
				
				//Turn on lights/siren
				return;
			} else {
				return; //Not a friend.
			}
		}
	}
    
}

void send_help_response(Friend *f, LocalUser *self, bool accept) {
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	//printf("PACKET:");
	//printBuff(f->networkaddr, 8, "%c");
	//memcpy(n->dest, f->networkaddr, 8);
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = HELP_RESPONSE;
	CLR_FLAGS(p->flags);
	if (accept) {
		SET_ACCEPT(p->flags);
	} else {
		CLR_ACCEPT(p->flags);
	}
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, p->opcode);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
}

void help_response_handler(Packet *p) {
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}	if (IS_ACK(p->flags)) {
		printv("HELP_RESP_HANDLER ACK\n");
		//Display to user whether they accepted or not.
		//If accepted, all good, otherwise, do a send to the next friend,
		//or HELP_REQUEST_ANYONE
		
		//Different light/alarm pattern
	} else {
		printv("HELP_RESP_HANDLER\n");
		
	}
}

void help_request_anyone_handler(Packet *p);
void help_response_anyone_handler(Packet *p);
void find_hops_request_handler(Packet *p);
void find_hops_response_handler(Packet *p);
void find_neighbors_request_handler(Packet *p);
void find_neighbors_response_handler(Packet *p);

void send_friend_request(Friend *f, LocalUser *self) {
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FRIEND_REQUEST;
	CLR_FLAGS(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, PING_REQUEST);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
}

void send_friend_request_ack(Friend *f, LocalUser *self) {
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FRIEND_REQUEST;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Add a corresponding message
	//Message *m = malloc(sizeof(Message));
	//setSettingsByOpcode(m, PING_REQUEST);
	//memcpy(m->srcAddr, n->dest, 8);
	//addToQueue(m);
	free(n);
	free(p);
}

void friend_request_handler(Packet *p) {
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printv("FRIEND_REQ_HANDLER ACK\n");
		//Check if already friend, and confirmed.
		
		//Add message/event for a FRIEND_RESPONSE.
	} else {
		printv("FRIEND_REQ_HANDLER\n");
		//Check if already friend. If already friend, drop packet.
	
		//Send ACK back.
		
		//Message user that someone wants to be a friend.
		
		//Add message/event for a FRIEND_RESPONSE ACK (Only after user accepts) //TODO NOT HERE.
		//If they accept, send a packet to them.
	}
}

void friend_response_handler(Packet *p) {
	if (IS_ACK(p->flags)) {
		printv("FRIEND_RESP_HANDLER ACK\n");
		//Do not need to do anything. Just confirms that it worked.
	} else {
		printv("FRIEND_RESP_HANDLER\n");
		//Check if in friend list
	
		//Confirm the friend.
	}
}

void unfriend_request_handler(Packet *p) {
	if (IS_ACK(p->flags)) {
		printv("UNFRIEND_REQ_HANDLER ACK\n");
		//Do not need to do anything.
	} else {
		printv("UNFRIEND_REQ_HANDLER\n");
		//Check if already a friend
	
		//Mark friend for deletion
		
		//Message user that someone has unfriended them
			
		//Send ACK packet back.
	}
}
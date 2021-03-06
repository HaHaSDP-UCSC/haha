/*
* packet.c
*
* Created: 4/23/2017 7:10:04 PM
*  Author: Brian, Kevin
*/

#include <stdlib.h>
#include <string.h>
#include "packet.h"
#include "network.h"
#include "flags.h"
#include "neighbor/responseflags.h"
#include "messagequeue/messagequeue.h"
#include "basecomm.h"
#include "neighbor/friendlist.h"
#include "utils/hahaUtils.h"
#include "lcd/lcd.h"
#include "menu/menu.h"
#include "menu/ui.h"
#include "lightsound/alarm/alarm.h"

static int friendReq = 0;

void opcodes_init() {
	haha_packet_handlers[PING_REQUEST] = ping_request_handler;
	haha_packet_handlers[HELP_REQUEST] = help_request_handler;
	haha_packet_handlers[HELP_RESPONSE] = help_response_handler;
	haha_packet_handlers[HELP_FROM_ANYONE_REQUEST] = help_request_anyone_handler;
	//haha_packet_handlers[HELP_FROM_ANYONE_RESPONSE] = help_response_anyone_handler;
	//haha_packet_handlers[FIND_HOPS_REQUEST] = find_hops_request_handler;
	//haha_packet_handlers[FIND_HOPS_RESPONSE] = find_hops_response_handler;
	haha_packet_handlers[FIND_NEIGHBORS_REQUEST] = find_neighbors_request_handler;
	haha_packet_handlers[FIND_NEIGHBORS_RESPONSE] = find_neighbors_response_handler;
	haha_packet_handlers[FRIEND_REQUEST] = friend_request_handler;
	haha_packet_handlers[FRIEND_RESPONSE] = friend_response_handler;
	haha_packet_handlers[UNFRIEND_REQUEST] = unfriend_request_handler;
}

//void app_packet_handler(char* data, uint16_t len, uint8_t* src, uint8_t id){
void app_packet_handler(Network *info) {
	HAHADEBUG("In app packet handler\n");
	opcode opcode = info->data[0];
	printd("Have Packet id %d with src:", info->id);
	printNetAddr(info->src);
	printd("\n");
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
	printf("after netarrayadd\n");
	printNetAddr(info->src);
	(haha_packet_handlers[p->opcode])(p);
	free(p);
}

void register_opcode_handler_function(opcode_handler_fn_t t, op opcode) {
	haha_packet_handlers[opcode] = t;
}

/* Helper Function */
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
	p->ORIGINUID = f->port; //These are not used in friends packets.
	strcpy(p->SRCFIRSTNAME, self->friend.firstname);
	strcpy(p->SRCLASTNAME, self->friend.lastname);
	strcpy(p->SRCHOMEADDR, self->homeaddr);
	strcpy(p->SRCPHONE, self->phoneaddr);
	p->ttl = 0; //TODO make this a proper number.
	p->id = 0; //TODO set this to the id number in the packet storage table.
}

/* Helper Function */
/* Returns network info from id attached to packet.*/
bool getNetInfo(Packet *p, Network *net) {
	printd("Finding netinfo packet id:%d\n", p->id);
	int i = netArrayReturn(p->id);
	if(i == NOT_FOUND){
		printd("ID %d not found.\n", p->id);
		return false;
	}
	printd("Returning id %d\n", p->id);
	//net = &NET_ARRAY[i];
	*net = NET_ARRAY[i];
	return true;
}

void send_ping_request(Friend *f) {
	printv("Send Ping Request\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	LocalUser *self = &localUsers[0]; //TODO make this scalable.
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = PING_REQUEST;
	CLR_FLAGS(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, PING_REQUEST);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
}

void ping_request_handler(Packet *p){
	printv("in ping handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if(IS_ACK(p->flags)){
		printd("IS Ping ACK!\n");
		//Lookup message/event
		Event *e = malloc(sizeof(Event) * MAXQUEUESIZE);
		printd("MESQ BEFORE LOOKING AT EVENTS\n");
		printMessageQueue();
		int numEntries = checkQueue(PING_REQUEST, p->flags, net, e);
		if(numEntries){
			printd("Found %d message for opcode %d\n", numEntries, p->opcode);
			printNetAddr(net->src);
			Friend *f = checkForFriend(net->src);
			f->lastresponse = queueTime;
		}
		removeFromQueueEvents(e, numEntries);
		free(e);
		printd("MESQ AFTER REMOVE QUEUE EVENTS\n");
		printMessageQueue();
	}
	else{
		printd("Is Ping Request\n");
		//Ping request network should still be in queue
		int i = netArrayReturn(p->id);
		if(i == NOT_FOUND){
			HAHADEBUG("net item not found\n");
			return;
		}
		net = &NET_ARRAY[i];
		//Packet* p2 = malloc(sizeof(Packet));
		
		net->dest = net->src;
		
		printd("Friend Lookup.\n");
		Friend *f = &friendList[0]; //TODO set this properly.
		LocalUser *self = &localUsers[0]; //TODO set this properly.
		Packet p2;
		p2.opcode = PING_REQUEST;
		CLR_FLAGS(p2.flags);
		SET_ACK(p2.flags);
		copy_friend_to_packet(f, self, &p2);

		sendPacket(&p2, net);
	}
}

void send_help_request(Friend *f, LocalUser *self){
	printv("Send Help Request\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	toglight = 1;
	p->opcode = HELP_REQUEST;
	CLR_FLAGS(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Add a corresponding message
	//Message *m = malloc(sizeof(Message));
	//setSettingsByOpcode(m, HELP_REQUEST);
	//memcpy(m->srcAddr, n->dest, 8);
	//addToQueue(m);
	free(n);
	free(p);
}

void send_help_request_ack(Friend *f, LocalUser *self) {
	printv("Send Help Request ACK\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->src = self->friend.networkaddr;
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = HELP_REQUEST;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	free(n);
	free(p);
}

void help_request_handler(Packet *p) {
	printv("Help Request Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	
	if (IS_ACK(p->flags)) {
		printd("HELP_REQ_HANDLER ACK\n");
		
		//Display to user that device was able to connect, pending response.
		//Add message/event for a HELP_RESPONSE ACK (Only after user accepts) //TODO NOT HERE.
		//DISPLAY USERNAME
		lcd_set_line(0, "Help Req Recv");
		} else {
		printd("HELP_REQ_HANDLER\n");
		//Check if friend
		int i = netArrayReturn(p->id);
		if(i == NOT_FOUND){
			HAHADEBUG("net item not found\n");
			return;
		}
		net = &NET_ARRAY[i];
		printd("INside of friedn req handler");
		printNetAddr(net->src);
		Friend *isFriend = checkForFriend(net->src);
		if (isFriend != NULL) {
			printd("Is a friend.\n");
			toglight =1;
			//Send HELP_REQUEST_ACK
			//menu->txtSrc1 = &localUsers[0].friend.firstname;
			strcpy(menu->txtSrc1, p->SRCFIRSTNAME);
			printf("\n%s->%s\n", menu->txtSrc1, p->SRCFIRSTNAME);
			menu->current = ui_item_helpresp;
			menu->current->onView();
			printf("\n%s->%s\n", menu->txtSrc1, p->SRCFIRSTNAME);
			send_help_request_ack(isFriend, &localUsers[0]); //TODO not zero
			//Display to user that friend is in need.
			
			//Turn on lights/siren
			setAlarm(true);
			
			} else {
			//Not a friend.
			printe("Not a friend.\n");
		}
	}
}

void send_help_response(Friend *f, LocalUser *self, bool accept) {
	printv("Send Help Response\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printv("********Sending to %s:", f->firstname);
	printNetAddr(n->dest);
	printv("\n");
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

void send_help_response_ack(Friend *f, LocalUser *self, bool accept) {
	printv("Send Help Response ACK\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = HELP_RESPONSE;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	if (accept) {
		SET_ACCEPT(p->flags);
		} else {
		CLR_ACCEPT(p->flags);
	}
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	free(n);
	free(p);
}

void help_response_handler(Packet *p) {
	printv("Help Response Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printd("HELP_RESP_HANDLER ACK\n");
		//Do not need to do anything.
		} else {
		printd("HELP_RESP_HANDLER\n");
		int i = netArrayReturn(p->id);
		if(i == NOT_FOUND){
			HAHADEBUG("net item not found\n");
			return;
		}
		net = &NET_ARRAY[i];
		printd("printing from helpreqhandler:");
		printNetAddr(net->src);
		Friend *isFriend = checkForFriend(net->src);
		if (isFriend != NULL) {
			printd("Is a friend.\n");
			bool accept = IS_ACCEPT(p->flags);
			LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
			
			//Display to user whether they accepted or not.
			//If accepted, all good, otherwise, do a send to the next friend,
			printd("HELP RESPONSE PACKET RECV! accept: %d\n", accept);
			lcd_clear();
			if (accept) {
				printd("Request Accepted.\n");
				lcd_set_line(0, "REQUEST ACCEPTED");
				char buf[64];
				sprintf(buf, "%s %s is on their way.", isFriend->firstname, 
				isFriend->lastname);
				lcd_set_line_overflow(1, buf);
				
			} else {
				printd("Request Denied.\n");
				lcd_set_line(0, "!    WARNING   !");
				char buf[64];
				sprintf(buf, "%s %s couldn't come.", isFriend->firstname, 
				isFriend->lastname);
				lcd_set_line_overflow(1, buf);
				//Send to next friend.
				friendReq++;
				if (friendReq < numFriends) {
					//TODO timeout timer to auto increment this on unresponsive people.
					printd("Calling next friend.\n");
					send_help_request(&friendList[friendReq], self);
				} else {
					printd("NO FRIENDS WERE AVAILABLE.\n");
					lcd_clear();
					lcd_set_line(0, "!    DANGER    !");
					lcd_set_line_overflow(1, "None of your    friends could   come help you!");
					friendReq = 0;
				}
			}
			lcd_update();
			
			//or HELP_REQUEST_ANYONE
			//Different light/alarm pattern
			
			//Send confirmation back.
			send_help_response_ack(isFriend, self, accept); //Just a confirmation.
			} else {
			//Drop.
		}
	}
}

//void help_request_anyone_handler(Packet *p);
void send_help_request_anyone(LocalUser *self){
	printv("Send Help Request\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = BROADCAST_ADDRESS();
	printNetAddr(n->dest);
	
	p->opcode = HELP_FROM_ANYONE_REQUEST;
	CLR_FLAGS(p->flags);
	//copy_friend_to_packet(f, self, p);
	sendPacketRadius(p, n, 5); //TODO Set radius to a proper number.
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, p->opcode);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
}

//No acks needed for help_request_anyone
//void send_help_request_anyone_ack(LocalUser *self, uint8_t radius);

void help_request_anyone_handler(Packet *p){
	printv("Help Request ANYONE Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
    	printe("Unable to get network info.\n");
    	return;
	}
	
	if (IS_ACK(p->flags)) {
		printd("HELP_REQ_ANYONE_HANDLER ACK\n");
		printe("Received unexpected ACK\n");
		return;
    } else {
		printd("HELP_REQ_ANYONE_HANDLER\n");
		//send_help_request_anyone_ack(&localUsers[0], DEFAULT_BCAST_RAD); //TODO not zero
		//TODO @brian@august Display to user that somebody is in need.
    
		//Turn on lights/siren
		setAlarm(true);
		
	}
}

void help_response_anyone_handler(Packet *p);
void find_hops_request_handler(Packet *p);
void find_hops_response_handler(Packet *p);

void send_find_neighbors_request() {
	printv("Send Find Neighbors Request\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	uint8_t *t = (uint8_t *) convert_asciihex_to_byte("000000000000FFFF");
	n->dest = t;
	printNetAddr(n->dest);
	
	p->opcode = FIND_NEIGHBORS_REQUEST;
	CLR_FLAGS(p->flags);
	//copy_friend_to_packet(f, self, p);
	
	sendPacket(p, n);
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, FIND_NEIGHBORS_RESPONSE); //Different than other responses because broadcast.
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
	
}

void send_find_neighbors_response(Friend *f, LocalUser *self, ttl ttl) {
	printv("Send Find Neighbors Response\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FIND_NEIGHBORS_RESPONSE;
	CLR_FLAGS(p->flags);
	copy_friend_to_packet(f, self, p);
	p->ttl = ttl; // Set the TTL TODO eventually put in better place.
	sendPacket(p, n);
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, p->opcode);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
}

void find_neighbors_request_handler(Packet *p) {
	printv("Find Neighbors Request Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printd("FIND_NEIGHBORS_HANDLER ACK\n");
		printe("Received unexpected ACK\n");
		//That is an error. Do nothing.
	} else {
		printNetAddr(net->src);
		printd("FIND_NEIGHBORS_REQ_HANDLER\n");
		LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
		Friend f; //Incomplete f. TODO this is probably okay.
		f.port = p->ORIGINUID; //Return UID;
		printd("FIND NEIGHBORS req SEND ADDR:");
		int i = netArrayReturn(p->id);
		if(i == NOT_FOUND){
			HAHADEBUG("net item not found\n");
			return;
		}
		net = &NET_ARRAY[i];
		printNetAddr(net->src);
		printd("End of address\n");
		memcpy(f.networkaddr, net->src, MAXNETADDR); //TODO @brian is this correct src? want to send to other station.
		send_find_neighbors_response(&f, self, net->ttl); //Sends this devices ttl so other can calculate.
	}
}

void send_find_neighbors_response_ack(Friend *f, LocalUser *self) {
	printv("Send Find Neighbors Response ACK\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FIND_NEIGHBORS_RESPONSE;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	free(n);
	free(p);
}

void find_neighbors_response_handler(Packet *p) {
	printv("Find Neighbors Response Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printd("FIND_NEIGHBORS_HANDLER ACK\n");
		//Do nothing. Just confirmation message.
	} else {
		printd("FIND_NEIGHBORS_HANDLER\n");
		int i = netArrayReturn(p->id);
		if(i == NOT_FOUND){
			HAHADEBUG("net item not found\n");
			return;
		}
		net = &NET_ARRAY[i];
		addNeighbor(p, net, queueTime);
		
		LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
		Friend f; //Incomplete f. TODO this is probably okay.
		f.port = p->ORIGINUID; //Return UID;
		memcpy(f.networkaddr, net->src, MAXNETADDR); //TODO @brian is this correct src? want to send to other station.
		send_find_neighbors_response_ack(&f, self);
	}
}

void send_friend_request(Friend *f, LocalUser *self) {
	printv("Send Friend Request\n");
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
	setSettingsByOpcode(m, p->opcode);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
}

void send_friend_request_ack(Friend *f, LocalUser *self) {
	printv("Send Friend Request ACK\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FRIEND_REQUEST;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	free(n);
	free(p);
}

void friend_request_handler(Packet *p) {
	printv("Friend Request Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printd("FRIEND_REQ_HANDLER ACK\n");
		//Check if already friend, and confirmed.
		Friend *isFriend = checkForFriend(net->src);
		if (isFriend != NULL) {
			printd("Is a friend.\n");
			//Add message/event for a FRIEND_RESPONSE.
			Message *m = malloc(sizeof(Message));
			setSettingsByOpcode(m, FRIEND_RESPONSE);
			memcpy(m->srcAddr, net->dest, 8);
			addToQueue(m);
		} else {
			printe("Not a friend but received this somehow.\n");
		}
	} else {
		printd("FRIEND_REQ_HANDLER\n");
		//Check if already friend. If already friend, drop packet.
		int i = netArrayReturn(p->id);
		if(i == NOT_FOUND){
			HAHADEBUG("net item not found\n");
			return;
		}
		net = &NET_ARRAY[i];
		printNetAddr(net->src);
		Friend *isFriend = checkForFriend(net->src);
		if (isFriend != NULL) {
			//Already a friend.
			printd("Is a friend. returning\n");
			return;
		}
		printd("Not a friend continuing\n");
		Friend f;
		f.id = numFriends; //TODO fix
		f.port = p->SRCUID;
		f.priority = numFriends; //TODO fix
		CLR_RESACCEPT(f.responseflag);
		strcpy(f.firstname, p->SRCFIRSTNAME);
		strcpy(f.lastname, p->SRCLASTNAME);
		memcpy(f.networkaddr, net->src, MAXNETADDR); //TODO @brian is this correct src? want to send to other station.
		f.lastresponse = 0; //TODO set to current time
		
		LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
		
		//addFriend(&f); //Add a friend that is not registered yet.
		//Message user that someone wants to be a friend.
		/*
		TODO @brian@august LCD Prompt	//Add message/event for a FRIEND_RESPONSE ACK (Only if user accepts) //TODO NOT HERE.
		If they accept, send a packet to them.
		*/
		printd("awaiting friend\n");
		awaitingAccept = f;
		printd("getting ready menu\n");
		menu->current = ui_item_friendresp;
		printd("menu set\n");
		menu->current->onView();
		printd("menu return\n");
		//Send ACK back.
		send_friend_request_ack(&f, self);
		//lcd_clear();
		//lcd_set_line(0, "Friend Req");
		//char buff[16];
		//sprintf(buff, "%s", p->SRCFIRSTNAME);
		//lcd_set_line(1, buff);
		//lcd_update();
		Message *m = malloc(sizeof(Message));
		setSettingsByOpcode(m, FRIEND_RESPONSE);
		//m->srcid = p->SRCUID;
		memcpy(m->srcAddr, net->dest, 8);
		addToQueue(m);
	}
}

void send_friend_response(Friend *f, LocalUser *self, bool accept) {
	printv("Send Friend Response\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FRIEND_RESPONSE;
	CLR_FLAGS(p->flags);
	if (accept)
		SET_ACCEPT(p->flags);
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

void send_friend_response_ack(Friend *f, LocalUser *self, bool accept) {
	printv("Send Friend Response ACK\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = FRIEND_RESPONSE;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	free(n);
	free(p);
}

void friend_response_handler(Packet *p) {
	printv("Unfriend Response Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	
	if (IS_ACK(p->flags)) {
		printv("FRIEND_RESP_HANDLER ACK\n");
		//Do not need to do anything. Just confirms that it worked.
		} else {
		printv("FRIEND_RESP_HANDLER\n");
		//Check if already a friend
		Friend *f = checkForFriend(net->src);
		if (f != NULL) {
			//Is sorta friend.
			SET_RESACCEPT(f->responseflag); //Now a full friend.
		}
		//Confirm the friend.
		LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
		send_friend_response_ack(f, self, IS_ACCEPT(p->flags));
	}
}

//TODO @brian @august call this in a menu item.
void send_unfriend_request(Friend *f, LocalUser *self) {
	//Send the request
	printv("Send Unfriend Request\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = UNFRIEND_REQUEST;
	CLR_FLAGS(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	//Unfriend friend.
	removeFriend(f);
	
	//Add a corresponding message
	Message *m = malloc(sizeof(Message));
	setSettingsByOpcode(m, p->opcode);
	memcpy(m->srcAddr, n->dest, 8);
	addToQueue(m);
	free(n);
	free(p);
}

void send_unfriend_request_ack(Friend *f, LocalUser *self) {
	//Send the request
	printv("Send Unfriend Request ACK\n");
	Network* n = malloc(sizeof(Network));
	Packet* p = malloc(sizeof(Packet));
	
	n->dest = f->networkaddr;
	printNetAddr(n->dest);
	
	p->opcode = UNFRIEND_REQUEST;
	CLR_FLAGS(p->flags);
	SET_ACK(p->flags);
	copy_friend_to_packet(f, self, p);
	sendPacket(p, n);
	
	free(n);
	free(p);
}

void unfriend_request_handler(Packet *p) {
	printv("Unfriend Request Handler\n");
	Network *net = NULL;
	if (!getNetInfo(p, net)) {
		printe("Unable to get network info.\n");
		return;
	}
	if (IS_ACK(p->flags)) {
		printd("UNFRIEND_REQ_HANDLER ACK\n");
		//Do not need to do anything.
		} else {
		printd("UNFRIEND_REQ_HANDLER\n");
		//Check if already a friend
		Friend *f = checkForFriend(net->src);
		//Message user that someone has unfriended them LCD @brian @august
		
		//Send ACK packet back.
		LocalUser *self = &localUsers[0]; //TODO Set this to something scalable.
		send_unfriend_request_ack(f, self);
		
		//Mark friend for deletion
		removeFriend(f);
	}
}
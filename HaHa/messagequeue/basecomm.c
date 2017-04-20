/**
 * @file basecomm.c
 * @brief Application layer packet creation and parsing.
 * @author Kevin Lee
 * @version
 * @date 2017-03-07
 */

#include "basecomm.h"

#define MINPACKETSIZE 4 //TODO define better to be based off of values

typedef struct {
	bool srcuid;
	bool destuid;
	bool originuid;
	bool srcname;
	bool srchomeaddr;
	bool srcphone;
	bool ttl;
} setPacketFields;

bool convertFromPacketToData(Packet *p, unsigned char *data);
bool convertFromDataToPacket(Packet *p, unsigned char *data, int datalen);

/**
 * Initializes the network.
 */
bool init_network() {
	/*
	if (_init_network(listenport) <= 0) {
		return false;
	}
	return true;*/
	return false; //TODO @brian hook xbee_init_network into this.
}

/**
 * Take the packet data and send to network device.
 * Returns if successful or not.
 */
bool sendPacket(Packet *p, Network *netdest) {
	unsigned char data[MAXBUFFER];
	int datalen = 0;
	if ((datalen = convertFromPacketToData(p, data)) <= 0) {
		printe("Error converting packet.\n");
		return false;
	}

	//TODO @brian hook xbee_sendPacket into this.
	/**
	if (_send_packet((char *) data, datalen, "127.0.0.1", dest) <= 0) {
		printe("Error sending packet.\n");
		return false;
	}
	return true;
	*/
	return false; //TODO FIX
}

/**
 * Checks for a packet on the network layer. This then processes the packet.
 */
bool recvPacket(Packet *p, Network *netsrc) {
	
	//TODO @brian hook xbee_sendPacket into function.
	
	/**
	//char buffer[BUFFERSIZE];
	//int receivelen;
	if ((receivelen = _recv_packet(buffer, BUFFERSIZE)) <= 0) {
		//printe("No packet to receive.\n");
		return false;
	}
	convertFromDataToPacket(p, (unsigned char *) buffer, receivelen);
	return true;
	*/
	//TODO @MESSAGEQ send to message queue.
	return false;
}

/**
 * Creates a packet from a standardized structure to be sent.
 */
bool formPacketToData(Packet *p, unsigned char *data, int *oldoffset,
		setPacketFields fields) {
	int n = 0;
	int offset = *oldoffset;
	if (fields.srcuid) {
		data[offset++] = p->SRCUID >> 8; //Add UPPER SRCUID to packet.
		data[offset++] = p->SRCUID; //Add LOWER SRCUID to packet.
		printd("Expected SRCUID: 0x%x\n", p->SRCUID);
		printd("Expected SRCUID: u%u\n", p->SRCUID);
		printd("SRCUID: 0x%x 0x%x\n", data[offset - 2], data[offset - 1]);
	}

	if (fields.destuid) {
		data[offset++] = p->DESTUID >> 8; //Add UPPER DESTUID to packet.
		data[offset++] = p->DESTUID; //Add LOWER DESTUID to packet.
		printd("Expected DESTUID: 0x%x\n", p->DESTUID);
		printd("Expected DESTUID: u%u\n", p->DESTUID);
		printd("DESTUID: 0x%x 0x%x\n", data[offset - 2], data[offset - 1]);
	}

	if (fields.originuid) {
		data[offset++] = p->ORIGINUID >> 8; //Add UPPER ORIGINUID to packet.
		data[offset++] = p->ORIGINUID; //Add LOWER ORIGINUID to packet.
		printd("Expected ORIGINUID: 0x%x\n", p->ORIGINUID);
		printd("Expected ORIGINUID: 0u%u\n", p->ORIGINUID);
		printd("ORIGINUID: 0x%x 0x%x\n", data[offset - 2], data[offset - 1]);
	}

	if (fields.ttl) {
		data[offset++] = p->ttl;
	}

	if (fields.srcname) {
		if ((n = strlen(p->SRCFIRSTNAME)) < MAXFIRSTNAME - 1) {
			printd("SRCFNAME: %s\nstrlen: %d\n", p->SRCFIRSTNAME, n);
			strcpy((char *) &data[offset], p->SRCFIRSTNAME); //Add SRCNAME to packet. TODO Optimize
			offset += n + 1; //String length + null terminator.
		} else {
			printe("Malformed First Name.\n");
			return false;
		}
		if ((n = strlen(p->SRCLASTNAME)) < MAXLASTNAME - 1) {
			printd("SRCLNAME: %s\nstrlen: %d\n", p->SRCLASTNAME, n);
			strcpy((char *) &data[offset], p->SRCLASTNAME); //Add SRCNAME to packet. TODO Optimize
			offset += n + 1; //String length + null terminator.
		} else {
			printe("Malformed Last Name.\n");
			return false;
		}
	}

	if (fields.srchomeaddr) {
		if ((n = strlen(p->SRCHOMEADDR)) < MAXHOMEADDR - 1) {
			printd("SRCHOMEADDR: %s\nstrlen: %d\n", p->SRCHOMEADDR, n);
			strcpy((char *) &data[offset], p->SRCHOMEADDR); //Add SRCHOMEADDR to packet. TODO Optimize
			offset += n + 1; //String length + null terminator.
		} else {
			printe("Malformed Home Address.\n");
			return false;
		}
	}

	if (fields.srcphone) {
		if ((n = strlen(p->SRCPHONE)) < MAXPHONE - 1) {
			printd("SRCPHONE: %s\nstrlen: %d\n", p->SRCPHONE, n);
			strcpy((char *) &data[offset], p->SRCPHONE); //Add SRCNAME to packet. TODO Optimize
			offset += n + 1; //String length + null terminator.
		} else {
			printe("Malformed Phone Number.\n");
			return false;
		}
	}

	*oldoffset = offset; //Make the old offset new.
	return true;
}

/**
 * Creates a packet from a standardized structure to be received.
 */
bool formDataToPacket(Packet *p, unsigned char *data, int *oldoffset,
		setPacketFields fields) {
	int n = 0;
	int offset = *oldoffset;

	if (fields.srcuid) {
		p->SRCUID = data[offset++] << 8;
		p->SRCUID += data[offset++]; //Add SRCUID to packet.
		printd("SRCUID: %u\n", p->SRCUID);
	}

	if (fields.destuid) {
		p->DESTUID = data[offset++] << 8;
		p->DESTUID += data[offset++]; //Add DESTUID to packet.
		printd("DESTUID: %u\n", p->DESTUID);
	}

	if (fields.originuid) {
		p->ORIGINUID = data[offset++] << 8;
		p->ORIGINUID += data[offset++]; //Add ORIGINUID to packet.
		printd("ORIGINUID: %u\n", p->ORIGINUID);
	}

	if (fields.ttl) {
		p->ttl = data[offset++];
	}

	if (fields.srcname) {
		if ((n = strlen((char *) &data[offset])) < MAXFIRSTNAME - 1) {
			strcpy(p->SRCFIRSTNAME, (char *) &data[offset]); //Add SRCNAME to packet. TODO Optimize
			offset += n + 1; //String length + null terminator.
			printd("SRCFNAME: %s\nstrlen: %d\n", p->SRCFIRSTNAME, n);
		} else {
			printe("Malformed First Name String.\n");
			return false;
		}
		if ((n = strlen((char *) &data[offset])) < MAXLASTNAME - 1) {
			strcpy(p->SRCLASTNAME, (char *) &data[offset]); //Add SRCNAME to packet. TODO Optimize
			offset += n + 1; //String length + null terminator.
			printd("SRCLNAME: %s\nstrlen: %d\n", p->SRCLASTNAME, n);
		} else {
			printe("Malformed Last Name String.\n");
			return false;
		}
	}

	if (fields.srchomeaddr) {
		if ((n = strlen((char *) &data[offset])) < MAXHOMEADDR - 1) {
			strcpy(p->SRCHOMEADDR, (char *) &data[offset]); //Add SRCHOMEADDR to packet
			offset += n + 1; //String length + null terminator.
			printd("SRCHOMEADDR: %s\nstrlen: %d\n", p->SRCHOMEADDR, n);
		} else {
			printe("Malformed Home Address String.\n");
			return false;
		}

	}

	if (fields.srcphone) {
		if ((n = strlen((char *) &data[offset])) < MAXPHONE - 1) {
			strcpy(p->SRCPHONE, (char *) &data[offset]); //Add SRCPHONE to packet
			offset += n + 1; //String length + null terminator.
			printd("SRCPHONE: %s\nstrlen: %d\n", p->SRCPHONE, n);
		} else {
			printe("Malformed Phone Number String.\n");
			return false;
		}
	}

	*oldoffset = offset; //Make the old offset new.
	return true;
}

/**
 * Processes Packet data to send on network. Converts into byte data.
 * They are processed based on their opcode.
 */
bool convertFromPacketToData(Packet *p, unsigned char *data) {
	printd("Convert Packet to Data.\n");

	if (p == NULL) {
		//Malformed packet.
		printe("Null Packet.\n");
		return false;
	}

	unsigned char opcode = p->opcode;
	unsigned char flags = p->flags;
	setPacketFields fields = { 0 }; //Boolean fields for adding modules.
		//TODO change this to false instead of 0, test to make sure it works.

	int offset = 0;
	data[offset++] = opcode;
	data[offset++] = flags;

	printd("Opcode: 0x%x\n", data[offset - 2]);
	printd("Flags: 0x%x\n", data[offset - 1]);

	int success = 0;
	switch (opcode) {
	case PING_REQUEST:
		printd("PING OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.destuid = true; //Add DESTUID
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID
			fields.srcname = true; //Add SRCNAME
			success = formPacketToData(p, data, &offset, fields);
		}
		break;

	case HELP_REQUEST:
		printd("HELP REQ OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.srchomeaddr = true; //Add SRCHOMEADDR to packet.
			fields.srcphone = true; //Add SRCPHONE
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			success = formPacketToData(p, data, &offset, fields);
		}
		break;

	case HELP_RESPONSE:
		printd("HELP RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			success = formPacketToData(p, data, &offset, fields);

		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			success = formPacketToData(p, data, &offset, fields);

		}
		break;

		//TODO:BROADCAST
	case HELP_FROM_ANYONE_REQUEST:
		printd("HELP FROM ANYONE REQUEST OPCODE.\n");
		fields.srcuid = true; //Add SRCUID to packet.
		fields.ttl = true; //Add TTL to packet.
		fields.srcname = true; //Add SRCNAME to packet.
		success = formPacketToData(p, data, &offset, fields);
		break;

	case HELP_FROM_ANYONE_RESPONSE:
		printd("HELP FROM ANYONE RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID
			fields.srcname = true; //Add SRCNAME
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srchomeaddr = true; //Add SRCHOMEADDR to packet.
			fields.srcphone = true; //Add SRCPHONE to packet.
			success = formPacketToData(p, data, &offset, fields);
		}
		break;

		//TODO:BROADCAST
	case FIND_HOPS_REQUEST:
		printd("FIND HOPS REQUEST OPCODE.\n");
		fields.originuid = true; //Add ORIGINUID to packet.
		fields.ttl = true; //Add TTL to packet.
		success = formPacketToData(p, data, &offset, fields);
		break;

	case FIND_HOPS_RESPONSE:
		printd("FIND HOPS RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SCRUID to packet.
			fields.originuid = true; //Add ORIGINUID to packet.
			fields.ttl = true; //Add TTL to packet.
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Addd SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formPacketToData(p, data, &offset, fields);
		}
		break;

		//TODO:BROADCAST
	case FIND_NEIGHBORS_REQUEST:
		printd("FIND NEIGHBORS REQUEST OPCODE.\n");
		fields.originuid = true; //Add ORIGINUID to packet.
		fields.ttl = true; //Add TTL to packet.
		success = formPacketToData(p, data, &offset, fields);
		break;

	case FIND_NEIGHBORS_RESPONSE:
		printd("FIND NEIGHBORS RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.originuid = true; //Add ORIGINUID to packet.
			fields.ttl = true; //Add TTL to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formPacketToData(p, data, &offset, fields);
		}
		break;

	case FRIEND_REQUEST:
		printd("FRIEND REQUEST OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formPacketToData(p, data, &offset, fields);
		}
		break;

	case FRIEND_RESPONSE:
		printd("FRIEND RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formPacketToData(p, data, &offset, fields);

		}
		break;

	case UNFRIEND_REQUEST:
		printd("UNFRIEND REQUEST OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formPacketToData(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formPacketToData(p, data, &offset, fields);

		}
		break;

	default:
		printe("Unimplemented OPCODE.\n");
		break;
	}

	if (!success) {
		printe("convertFromPacketToData Failed.\n");
		return false;
		//TODO Do something more important.
	}

	printd("Exiting packet to data conversion.\n");
	printd("Offset: %d.\n", offset);
	return offset;
}

/**
 * Processes received data from network. Converts into packet form.
 * They are processed based on their opcode.
 */
bool convertFromDataToPacket(Packet *p, unsigned char *data, int datalen) {
	printd("Convert Data to Packet.\n");
	printf("Datalen: %d\n", datalen);

	if (datalen < MINPACKETSIZE) {
		//Malformed packet.
		printe("Packet too small.\n");
		return false;
	}

	int offset = 0;
	unsigned char opcode = data[offset++];
	unsigned char flags = data[offset++];
	setPacketFields fields = { 0 }; //Boolean fields for adding modules.

	p->opcode = opcode;
	p->flags = flags;

	printd("Opcode: 0x%x\n", opcode);
	printd("Flags: 0x%x\n", flags);

	//TODO Do a check to see if this packet needs to be processed.
	//If the packet is not expected, drop the packet.
	/**
	 * if (!isExpecting(opcode, PING_REQUEST_FLAG)) {
	 *  //drop packet
	 * }
	 */
	int success = 0;

	switch (opcode) {
	case PING_REQUEST:
		printd("PING OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.destuid = true; //Add DESTUID
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID
			fields.srcname = true; //ADD SRCNAME
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

	case HELP_REQUEST:
		printd("HELP REQ OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.srchomeaddr = true; //Add SRCHOMEADDR to packet.
			fields.srcphone = true; //Add SRCPHONE to packet
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

	case HELP_RESPONSE:
		printd("HELP RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

		//TODO:BROADCAST
	case HELP_FROM_ANYONE_REQUEST:
		printd("HELP FROM ANYONE REQUEST OPCODE.\n");
		fields.srcuid = true; //Add SRCUID
		fields.ttl = true; //Add TTL to packet.
		fields.srcname = true; //Add SRCNAME to packet.
		success = formDataToPacket(p, data, &offset, fields);
		break;

	case HELP_FROM_ANYONE_RESPONSE:
		printd("HELP FROM ANYONE RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srchomeaddr = true; //Add SRCHOMEADDR to packet
			fields.srcphone = true; //Add SRCPHONE to packet
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

		//TODO:BROADCAST
	case FIND_HOPS_REQUEST:
		printd("FIND HOPS REQUEST OPCODE.\n");
		fields.originuid = true; //Add ORIGINUID to packet.
		fields.ttl = true; //Add TTL to packet.
		success = formDataToPacket(p, data, &offset, fields);
		break;

	case FIND_HOPS_RESPONSE:
		printd("FIND HOPS RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.originuid = true; //Add ORIGINUID to packet
			fields.ttl = true; //Add TTL to packet.
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

		//TODO:BROADCAST
	case FIND_NEIGHBORS_REQUEST:
		printd("FIND NEIGHBORS REQUEST OPCODE.\n");
		fields.originuid = true; //Add ORIGINUID to packet.
		fields.ttl = true; //Add TTL to packet.
		success = formDataToPacket(p, data, &offset, fields);
		break;

	case FIND_NEIGHBORS_RESPONSE:
		printd("FIND NEIGHBORS RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.originuid = true; //Add ORIGINUID to packet
			fields.ttl = true; //Add TTL to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

	case FRIEND_REQUEST:
		printd("FRIEND REQUEST OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			fields.srcname = true; //Add SRCNAME to packet.
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

	case FRIEND_RESPONSE:
		printd("FRIEND RESPONSE OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

	case UNFRIEND_REQUEST:
		printd("UNFRIEND REQUEST OPCODE.\n");
		if (!IS_ACK(flags)) {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formDataToPacket(p, data, &offset, fields);
		} else {
			fields.srcuid = true; //Add SRCUID to packet.
			fields.destuid = true; //Add DESTUID to packet.
			success = formDataToPacket(p, data, &offset, fields);
		}
		break;

	default:
		printe("Unhandled OPCODE.\n");
		//TODO Undefined.
		return false;
	}
	if (!success) {
		printe("convertFromPacketToData Failed.\n");
		return false;
		//TODO Do something more important.
	}
	printd("Exiting data to packet conversion.\n");
	printd("Offset: %d.\n", offset);
	return offset;
}

//TODO @brian What does this do? 
bool networkUp() {
	return false; //TODO fix.
}


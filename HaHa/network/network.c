/*
 * @file network.c
 * @brief Definition of different network layers. Net data passed through this.
 * @author Brian Nichols
 * @date 4/23/2017 4:37:47 PM
 */ 
#include "network.h"
#include "networkdevice/networkdevice.h"
#include "utils/hahaUtils.h"
//#include "packet.h"
//#include "stdlib.h"
//#include "string.h"

void printNetAddr(netaddr t) {
	printf("NetAddr:[");
	printBuff(t, 8, "%c");
	printf("][");
	printBuff(t, 8, "%2x");
	printf("]\n");
	}

void network_init(){
    xbee_init();
    opcodes_init();
    myNetAddr = "0013A200414f50e9"; //get from xbee function
}

void netArrayAdd(Network* add){
	printd("Adding NetInfo id %d with src:", add->id);
	printNetAddr(add->src);
	printd("\n");
    NET_ARRAY[netIDLast] = *add;
    netIDLast = (netIDLast + 1) % MAX_NET_ARRAY;
}

/* Return index of net array with id netID */
uint8_t netArrayReturn(uint8_t netID){
	printd("Searching Net Array\n");
    for(int i=0; i<MAX_NET_ARRAY; ++i){
		printd("Searching id: %d addr: [", i);
		printNetAddr(NET_ARRAY[i].src);
		printd(",dst:");
		printNetAddr(NET_ARRAY[i].dest);
		printd("]\n");
        if(NET_ARRAY[i].id == netID)
            return i;
    }
    return NOT_FOUND;
}

bool netCompare(netaddr n1, netaddr n2){
    printd("Comparing ");
    //printBuff(n1, 8, "%x");
	//printNetAddr(n1);
	printd("n1:");
	printBuff(n1, 8, "%x");
    printd(" to ");
    printBuff(n2, 8, "%x");
	//printNetAddr(n2);
    printd("\n");
    for(int i=0; i<MAXNETADDR; ++i){
        //printd("testing %d", i);
		printf("n2[i]:%x\n", n2[3]);
        printd("testing %x %x", n1[i], n2[i]);
        //if(n1[i] != n2[i]){
            //printd("index %d no match\n", i);
            //return false;
        //}            
    }
    printd("netCompare true\n");
    return true;
}

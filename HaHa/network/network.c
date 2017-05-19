/*
 * @file network.c
 * @brief Definition of different network layers. Net data passed through this.
 * @author Brian Nichols
 * @date 4/23/2017 4:37:47 PM
 */ 
#include "network.h"
#include "networkdevice/networkdevice.h"
//#include "packet.h"
//#include "stdlib.h"
//#include "string.h"


void network_init(){
    xbee_init();
    opcodes_init();
    myNetAddr = "0013A200414f50e9"; //get from xbee function
}

void netArrayAdd(Network* add){
    NET_ARRAY[netIDLast] = *add;
    netIDLast = (netIDLast + 1) % MAX_NET_ARRAY;
}

/* Return index of net array with id netID */
uint8_t netArrayReturn(uint8_t netID){
    for(int i=0; i<MAX_NET_ARRAY; ++i){
        //if(NET_ARRAY[i].id == netID)
            return i;
    }
    return NOT_FOUND;
}

bool netCompare(netaddr n1, netaddr n2){
    printd("Comparing ");
    //printBuff(n1, 8, "%x");
    printd(" to ");
    //printBuff(n2, 8, "%x");
    printd("\n");
    //for(int i=0; i<MAXNETADDR; ++i){
        //printd("testing %d", i);
        //printd("testing %d %d", n1[i], n2[i]);
        ////if(n1[i] != n2[i]){
        ////    printd("index %d no match\n", i);
        ////    return false;
        ////}            
    //}
    printd("netCompare true\n");
    return true;
}

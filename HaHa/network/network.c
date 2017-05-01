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
    myNetID = "0013A200414f50e9"; //get from xbee function
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

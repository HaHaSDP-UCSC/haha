/*
 * @file network.c
 * @brief Definition of different network layers. Net data passed through this.
 * @author Brian Nichols
 * @date 4/23/2017 4:37:47 PM
 */ 
#include "network.h"
#include "networkdevice/networkdevice.h"
#include "packet.h"
#include "stdlib.h"
#include "string.h"


void network_init(){
    xbee_init();
    opcodes_init();
}       
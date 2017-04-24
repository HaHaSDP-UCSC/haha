/*
 * network.c
 *
 * Created: 4/23/2017 7:07:33 PM
 *  Author: Brian
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
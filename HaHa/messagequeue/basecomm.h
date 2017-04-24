/**
 * @file basecomm.h
 * @brief Base communication
 * @author Kevin Lee
 * @version
 * @date 2017-03-07
 *
 * These functions will call the functions in network.h
 * The network.h file is the actual implementation of network comm.
 */

#ifndef _HA_BASECOMM
#define _HA_BASECOMM

#include "utils/hahaUtils.h"
#include "network/packet.h"
#include "networkdevice/networkdevice.h"
#include "network/network.h"
#include <string.h>

#define MAXBUFFER 100 //Maximum size of application layer packet. //TODO define

bool init_network(); //TODO @brian@kevin Unknown needed params

bool sendPacket(Packet *p, Network *net);
bool recvPacket(Packet *p, Network *net);

#endif

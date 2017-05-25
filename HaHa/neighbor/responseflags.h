/*
 * responseflags.h
 *
 * Created: 5/25/2017 3:49:12 AM
 *  Author: kevin
 */ 


#ifndef RESPONSEFLAGS_H_
#define RESPONSEFLAGS_H_

#include "utils/hahaUtils.h"

typedef uint8_t flags;

/* Flags */
#define RESFLAG_ACCEPT 0x01

#define SET_RESACCEPT(val) val |= FLAG_ACK

#define CLR_RESFLAGS(val) val = 0
#define CLR_RESACCEPT(val) val &=~ RESFLAG_ACCEPT

#define IS_RESACCEPT(val) val & RESFLAG_ACCEPT

#endif /* RESPONSEFLAGS_H_ */
#ifndef _HA_FLAGS
#define _HA_FLAGS

typedef unsigned char BYTE; /* Allow for device specifics */
typedef BYTE flags;
/* Flags */
#define FLAG_ACK 0x01
#define FLAG_ACCEPT 0x02
#define FLAG_CANCEL 0x04
#define FLAG_IMMEDIATE 0x08

#define SET_ACK(val) val |= FLAG_ACK
#define SET_ACCEPT(val) val |= FLAG_ACCEPT
#define SET_CANCEL(val) val |= FLAG_CANCEL
#define SET_IMMEDIATE(val) val |= FLAG_IMMEDIATE
#define CLR_ACK(val) val &=~ FLAG_ACK
#define CLR_ACCEPT(val) val &=~ FLAG_ACCEPT
#define CLR_CANCEL(val) val &=~ FLAG_CANCEL
#define CLR_IMMEDIATE(val) val &=~ FLAG_IMMEDIATE
#define CLR_FLAGS(val) val = 0
#define IS_ACK(val) val & FLAG_ACK
#define IS_ACCEPT(val) val & FLAG_ACCEPT
#define IS_CANCEL(val) val & FLAG_CANCEL
#define IS_IMMEDIATE(val) val & FLAG_IMMEDIATE

#endif

#ifndef _HA_UTILS
#define _HA_UTILS
/*
* utils.h
*
* Created: 3/8/2017 12:55:35 PM
*  Author: Brian
*/

#include <ctype.h>
#include <stdio.h>
#include <stdio_start.h>

#define bool	uint8_t
#define TRUE	1
#define FALSE	0

/*For debugging */
#define DEBUG_PRINT 1
#ifdef DEBUG_PRINT
#define DPRINTF(format, args...) printf(format, ## args)
#else
#define DPRINTF(format, args...)
#endif

/* Delays in ~ms */
 static void delay(uint32_t ms)
 {
	 for (uint32_t i = 0; i < ms; i++) {
		 for (uint32_t j = 0; j < 5000; j++)
		 asm volatile ("nop");
	 }
 }

/* Takes two char and converts to Hex */
uint8_t asciihex_to_byte(uint8_t d1, uint8_t d2);

#endif
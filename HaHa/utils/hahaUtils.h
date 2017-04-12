/**
 * \file
 *
 * \brief Utility functions/variables and debug functionality
 */
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

//#define bool	uint8_t
#define TRUE	1
#define FALSE	0

#define COLORED TRUE

//Color codes for text
#define CDEF  "\033[0m"
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CBLU  "\x1B[34m"
#define CYEL  "\x1B[33m"
#define CMAG  "\x1B[35m"
#define CCYN  "\x1B[36m"
#define CWHT  "\x1B[37m"

/*For debugging */
/**
 * { #define for debug printing }
 */
#define DEBUG_PRINT TRUE
#ifdef DEBUG_PRINT
#define HAHADEBUG(format, args...) printf(format, ## args)
#else
#define HAHADEBUG(format, args...)
#endif

/************************************************************************/
/* Quick Standard Error Print                                           */
/************************************************************************/
#if COLORED
#define printe(args...) fprintf(stderr, CRED); fprintf(stderr, args); fprintf(stderr, CDEF);
#else
#define printe(args...) fprintf(stderr, args);
#endif

//typedef enum printType {CHAR, HEX, INT};

/* Delays in ~ms */
/**
 * @brief      { Delays execution in ms }
 *
 * @param[in]  ms    The milliseconds
 */
void delay(uint32_t ms);
/**
 * @brief      { Print a buffer to stdout }
 *
 * @param      data       The data
 * @param[in]  len        The length
 * @param      specifier  The specifier
 */
void printBuff(char* data, uint8_t len, char* specifier);
/**
 * @brief      { Takes two char and converts to Hex }
 *
 * @param[in]  d1    First ascii character
 * @param[in]  d2    Second ascii character
 *                   
 * @return     { The converted hexadecimal value }
 */
uint8_t asciihex_to_byte(uint8_t d1, uint8_t d2);

#endif
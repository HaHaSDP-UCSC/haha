/**
 * @file hahaUtils.h
 * @brief stdlib for HAHA project, contains imports and defines
 * @author August, Kevin, Brian
 * @version
 * @date 2017-04-14
 */

#ifndef _HA_UTILS
#define _HA_UTILS

#include <stdbool.h>
#include <stdio.h>

/* Print preferences */
/* Set to false if you want to turn it off. */
#define COLORED    true /* Set this to false to disable terminal colors.  */
#define DEBUGMES   true /* Set this to false to disable debug messages.   */
#define VERBOSEMES true /* Set this to false to disable verbose messages. */
#define ERRORMES   true /* Set this to false to disable error messages.   */

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
#define DEBUG_PRINT true
#ifdef DEBUG_PRINT
#define HAHADEBUG(format, args...) printf(format, ## args)
#else
#define HAHADEBUG(format, args...)
#endif

/************************************************************************/
/* Quick Standard Error Print                                           */
/************************************************************************/
#if ERRORMES
#if COLORED
#define printe(args...) fprintf(stderr, CRED), fprintf(stderr, args), \
fprintf(stderr, CDEF);
#else
#define printe(args...) fprintf(stderr, args);
#endif // COLORED
#else
#define printe(...) (void)0;
#endif // ERRORMES

/************************************************************************/
/* Quick Standard Print Verbose Messages                                */
/************************************************************************/
#if VERBOSEMES
#if COLORED
#define printv(args...) fprintf(stdout, CGRN); fprintf(stdout, args); \
fprintf(stdout, CDEF);
#else
#define printv(args...) fprintf(stdout, args);
#endif // COLORED
#else
#define printv(...) (void)0;
#endif // VERBOSE

/************************************************************************/
/* Quick Standard Print Debug Messages                                  */
/************************************************************************/
#if DEBUGMES
#if COLORED
#define printd(args...) fprintf(stdout, CBLU); fprintf(stdout, args); \
fprintf(stdout, CDEF);
#else
#define printd(args...) fprintf(stdout, args)
#endif // COLORED
#else
#define printd(...) (void)0;
#endif // DEBUG

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

uint64_t byte_array_to_64(uint8_t* s);

#endif
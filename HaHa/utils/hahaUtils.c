/**
 * @file hahaUtils.c
 * @brief stdlib for HAHA project, contains imports and defines implementation
 * @author Brian
 * @version
 * @date 2017-03-17
 */
#include "utils/hahaUtils.h"
//#include <ctype.h>
//#include <stdio_start.h>

/* Delay in ms */
void delay(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++) {
        for (uint32_t j = 0; j < 5000; j++)
        asm volatile ("nop");
    }
}

/* Print out a buffer */
void printBuff(char* data, uint8_t len, char* specifier){
     for(int i=0; i<len; ++i){
        printf(specifier, data[i]);
    }
 }

 /* Takes two char and converts to Hex */
 uint8_t asciihex_to_byte(uint8_t d1, uint8_t d2){
     uint8_t aux=0;
     uint8_t aux2=0;
     uint8_t resul=0;
     d1 = toupper(d1);
     d2 = toupper(d2);
     switch(d1)
     {
         case 48: aux=0;
         break;
         case 49: aux=1;
         break;
         case 50: aux=2;
         break;
         case 51: aux=3;
         break;
         case 52: aux=4;
         break;
         case 53: aux=5;
         break;
         case 54: aux=6;
         break;
         case 55: aux=7;
         break;
         case 56: aux=8;
         break;
         case 57: aux=9;
         break;
         case 65: aux=10;
         break;
         case 66: aux=11;
         break;
         case 67: aux=12;
         break;
         case 68: aux=13;
         break;
         case 69: aux=14;
         break;
         case 70: aux=15;
         break;
     }
     switch(d2)
     {
         case 48: aux2=0;
         break;
         case 49: aux2=1;
         break;
         case 50: aux2=2;
         break;
         case 51: aux2=3;
         break;
         case 52: aux2=4;
         break;
         case 53: aux2=5;
         break;
         case 54: aux2=6;
         break;
         case 55: aux2=7;
         break;
         case 56: aux2=8;
         break;
         case 57: aux2=9;
         break;
         case 65: aux2=10;
         break;
         case 66: aux2=11;
         break;
         case 67: aux2=12;
         break;
         case 68: aux2=13;
         break;
         case 69: aux2=14;
         break;
         case 70: aux2=15;
         break;
         default: aux2=100;
         break;
     }
     if(aux2==100)
     {
         resul=aux;
     }
     else
     {
         resul=(aux*16)+aux2;
     }
     return resul;
 }
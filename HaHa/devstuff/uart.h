#ifndef _HA_UART
#define _HA_UART
/*
 * uart.h
 *
 * Created: 3/5/2017 5:35:25 PM
 *  Author: Brian
 */ 

 //#include "driver_examples.h"
 //#include "driver_init.h"
 //#include "atmel_start_pins.h"
 #include "stdio_start.h"

#define USART_BUF_SIZE 16
#define CB_OFF 0
#define CB_ON 1
#define SET_RX_CB(val)		(_RX_CALLBACK = val)
#define SET_RX_CB_OFF		SET_RX_CB(CB_OFF)
#define SET_RX_CB_ON		SET_RX_CB(CB_ON)
#define IS_RX_CB_ON			(_RX_CALLBACK == CB_ON)
#define SET_SEND_XBEE(val)	(SEND_XBEE = val);
uint8_t _RX_CALLBACK;
uint8_t SEND_XBEE;

struct io_descriptor *usart_io;
//uint8_t UART_BUFF;
//uint8_t *uart_buffer[2];
//uint8_t uart_buffer[USART_BUF_SIZE];
char uart_buffer[2][USART_BUF_SIZE];
//char _uartbuff0[USART_BUF_SIZE];
//char _uartbuff1[USART_BUF_SIZE];
uint8_t CURRENT_BUFFER;
//uint8_t uart_buffer2[USART_BUF_SIZE];
//uart_buffer[0] = uart_buffer1;
//uart_buffer[1] = uart_buffer2;
uint8_t data_received;
void uart_getbuffer(char* buff, size_t len);
uint8_t uart_read();
uint8_t uart_write(uint8_t *data, size_t size);
uint8_t uart_clearbuffer();
void uart_init_irqs(void);

#endif
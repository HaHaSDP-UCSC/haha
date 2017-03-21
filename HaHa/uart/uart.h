/**
 * \file
 *
 * \brief UART control functionality declaration
 */
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
/**
 * { Flag controller whether UART RX callback is used }
 */
uint8_t _RX_CALLBACK;
/**
 * { Flag on whether to send data to XBee receive function }
 */
uint8_t SEND_XBEE;

struct io_descriptor *usart_io;
char uart_buffer[2][USART_BUF_SIZE];
uint8_t CURRENT_BUFFER;
uint8_t data_received;
/**
 * @brief      { Returns the contents of the UART buffer }
 *
 * @param      buff  The buffer to put data into
 * @param[in]  len   The length the buffer
 */
void uart_getbuffer(char* buff, size_t len);
/**
 * @brief      { Reads data received by UART into rotating buffer }
 *
 * @return     { Number of bytes read }
 */
uint8_t uart_read();
/**
 * @brief      { Writes data out the UART }
 *
 * @param      data  The data
 * @param[in]  size  The size of data
 *
 * @return     { Number of bytes sent out UART }
 */
uint8_t uart_write(uint8_t *data, size_t size);
/**
 * @brief      { Clear the UART receive buffer }
 *
 * @return     { Returns 0 if successful }
 */
uint8_t uart_clearbuffer();
/**
 * @brief      { Initializes and registers interrupts for UART }
 */
void uart_init_irqs(void);

#endif
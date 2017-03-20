/*
 * uart.c
 *
 * Created: 3/5/2017 4:38:29 PM
 *  Author: Brian
 */ 
#include "uart.h"
#include "driver_init.h"
#include "devstuff/hahaUtils.h"
#include "xbee/xbee.h"
#include <hal_usart_async.h>
#include <string.h>

#define NUM_TX_BUFFS 2
#define TX_BUF_LEN   16
uint8_t txdone;
uint8_t txBuff[NUM_TX_BUFFS];

static void tx_cb_USART_1_0(const struct usart_async_descriptor *const io_descr)
{
    /* Transfer completed */
    //printf("txdone!");
    //txdone = 1;
}
static void rx_cb(const struct usart_async_descriptor *const io_descr)
{
    /* RX completed */
    //RX_CB_ON and _OFF can be used here for future
}

void uart_init_irqs(void)
{
    usart_async_register_callback(&USART_1_0, USART_ASYNC_TXC_CB, tx_cb_USART_1_0);
    usart_async_register_callback(&USART_1_0, USART_ASYNC_RXC_CB, rx_cb);
    /*usart_async_register_callback(&USART_1_0, USART_ASYNC_ERROR_CB, err_cb);*/
    usart_async_get_io_descriptor(&USART_1_0, &usart_io);
    usart_async_enable(&USART_1_0);
    SET_RX_CB_OFF;
    SET_SEND_XBEE(FALSE);
    CURRENT_BUFFER = 0;
}

uint8_t uart_write(uint8_t* data, size_t size){
    //printf("Writing To Uart:");
    //for(int i=0; i<size; ++i){
        //printf("%x", data[i]);
    //}

    //We block if the last one hasn't finished yet
    while(!usart_async_is_tx_empty(&USART_1_0));
    
    return io_write(usart_io, data, size);

    /////////////////////////////////
    /* A possible update for later */
    //If its larger than txbufflen we need to queue it
    //Send the first 16
    //if(size > 16){
        ////send the first buff len
        //io_write(usart_io, data, TX_BUF_LEN);
        ////queue the rest
        //uint8_t chunk_size = size/TX_BUF_LEN;
        //uint8_t rem = size - chunk_size *NUM_TX_BUFFS;
        //int start, end;
        //for(start=TX_BUF_LEN, end=TX_BUF_LEN+ chunk_size; 
            //start<size;
            //start = end, end=start+chunk_size)
        //{
            //
        //}
    //}
    //return io_write(usart_io, data, size);
}

//void print_data(uint8_t *t, int len ){
    //printf("Printing Received Data:\n");
    //for(int i=0; i<len; ++i)
    //printf("%c", t[i]);
    //printf("\n");
//}

uint8_t uart_read(){
    //printf("uart_read()");
    static int timeout = 0;
    data_received += io_read(usart_io, &uart_buffer[CURRENT_BUFFER][data_received],
    USART_BUF_SIZE - data_received);
    //if(data_received && SEND_XBEE){
        //printf("sendxbeerunning\n");
        //xbee_recv(uart_buffer, data_received);
        //data_received = 0;
    //}
    
    if(data_received >= USART_BUF_SIZE || (data_received && (timeout++ > 100000))) {
        HAHADEBUG("Processing UART Received Data.\n");
        if(SEND_XBEE){
            HAHADEBUG("Sending Received Data To Xbee Handler\n");
            xbee_recv(uart_buffer[CURRENT_BUFFER], data_received);
        }
        else{
            HAHADEBUG("Sending Received Data To Printer\n");
            //print_data(uart_buffer[CURRENT_BUFFER], data_received); // Process data
            printBuff(uart_buffer[CURRENT_BUFFER], data_received, "%c");
        }
        data_received = 0;
        CURRENT_BUFFER = !CURRENT_BUFFER;
        HAHADEBUG("UART CurrentBuff:%d[%x]->%d[%x]\n", !CURRENT_BUFFER,!CURRENT_BUFFER, CURRENT_BUFFER,CURRENT_BUFFER);
        timeout = 0;
    }
    return data_received;
}

void uart_getbuffer(char* buff, size_t len){
    memcpy(buff, uart_buffer, len);
}

uint8_t uart_clearbuffer(){
    uint8_t ret = data_received;
    data_received = 0;
    return ret;
}

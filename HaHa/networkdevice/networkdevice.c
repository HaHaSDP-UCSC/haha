/*
* xbee.c
*
* Created: 3/5/2017 6:03:43 PM
*  Author: Brian
*/
#include "networkdevice.h"
#include "uart/uart.h"
#include "utils/hahaUtils.h"
#include "network/network.h"
#include <string.h>


#define HOPS_NO_MAX 0
/* Internal Functions */
static void _rxFrame_clear(frameRX *p);
static uint8_t _xbee_send(char* data, uint8_t len, uint8_t radius);
static uint8_t _xbee_sendFrame(frameTX *tx);
static uint8_t _frame_TX_tochar(char* buff, frameTX *txData, int size);
static void _packet_Handler(frameIncoming * p);
static uint8_t _get_UART_Data(uint8_t numbytes);
static void _print_Packet(frameRX *p);
static void _print_FrameIncoming(frameIncoming *p);
uint8_t _verify_checksum(frameIncoming *f);
static void _checksum_fail();
static uint8_t _parseRX(frameIncoming *frame, frameRX *out);


/* Frame data */
const uint8_t startdelim = 0x7E;
frameIncoming recvBuff[2];
frameTX txData;
frameRX rxData;
uint8_t RXBUFF_CUR = 0;
uint8_t rxID;
uint8_t txID;


void xbee_init(){
    rxID = 0;
    txID = 1;
    //_rxFrame_clear(&recvBuff[0]);
    //_rxFrame_clear(&recvBuff[1]);
}

void netdevice_register_callback(netdevice_cb_t t, frameResponseType type){
    switch (type){
        case FRAME_MODEM_STATUS:
        case FRAME_TX_STATUS:
        case FRAME_ROUTE_INFO:
        case FRAME_AGG_ADDR:
        case FRAME_RX_EXPLICIT:
        case FRAME_DATA_SAMPE:
        case FRAME_NODE_ID:
        case FRAME_REMOTE_RESP:
                    break;
        case FRAME_RX:
                    rxData.callback = t;
                    break; 
        default:
                    HAHADEBUG("Invalid callback register type");
    }
    
}

void xbee_enterCmdMode(){
    //Send '+++' to enter command mode guarded by 1 sec on each side
    HAHADEBUG("Configuring XBee for API Mode...\n");
    delay(1000);
    uart_write("+++", 3);
    delay(1000);
    //Make sure returned 'OK<cr>' (get 3 next 3 bytes from UART)
    _get_UART_Data(3);
    //Clear the buffered message
    uart_clearbuffer();
    //SET_RX_CB_ON;
}

void xbee_leaveCmdMode(){
    HAHADEBUG("Xbee leaving command mode...\n");
    SET_RX_CB_OFF;
    uart_write("ATCN\r", 5);
    /* Get OK<CR> from UART */
    _get_UART_Data(3);
    SET_RX_CB_ON;
}

uint8_t cmd_AT_set(char* cmd, uint8_t* value, uint8_t len){
    uint16_t total_len = 4+len; //not including start, total, checksum
    char data[total_len + 4]; //include start, total, checksum
    data[0] = startdelim;
    data[1] = (total_len >> 8) & 0xFF;
    data[2] = total_len;
    data[3] = FRAME_AT;
    data[4] = txID++;
    
    data[5] = cmd[0];
    data[6] = cmd[1];
    for(int i=7; i<7+len; ++i)
        data[i] = value[i-7];
    data[7+len] = calc_checksum(data[3], total_len);
    int bytes = uart_write(data, total_len + 4);
    printf("wrote:%d bytes\n", bytes);
    
}
uint8_t cmd_AT_get(char* cmd){
    HAHADEBUG("Getting AT command:%s\n", cmd);
    uint16_t total_len = 4; //not including start, total, checksum
    char data[4 + total_len]; //cinldue start, total, checksum
    data[0] = startdelim;
    data[1] = (total_len >> 8) & 0xFF;
    data[2] = total_len;
    data[3] = FRAME_AT;
    data[4] = txID++;
    
    data[5] = cmd[0];
    data[6] = cmd[1];
    data[7] = calc_checksum(data+3, total_len);
    int bytes = uart_write(data, total_len + 4);
    //printf("wrote:%d bytes\n", bytes);
}

uint8_t calc_checksum(char *data, uint16_t len){
    HAHADEBUG("Calculating checksum\n");
    uint16_t total = 0;
    for(int i=0; i<len; ++i){
        total+= data[i];
    }
    HAHADEBUG("Checksum: %d - %d = %d\n", 0xFF, total, 0xFF-total);
    return 0xFF - total;
}

/* Exposed send function */
uint8_t netdevice_send(uint64_t dst, char* data, uint8_t datalen){
    netdevice_send_radius(dst, data, datalen, HOPS_NO_MAX);
}

uint8_t netdevice_send_radius(uint64_t dst, char* data, uint8_t datalen, uint8_t radius){
    HAHADEBUG("In netdevice_send\n");
    txData.dst[0] = (dst >> 56) & 0xFF;
    txData.dst[1] = (dst >> 48) & 0xFF;
    txData.dst[2] = (dst >> 40) & 0xFF;
    txData.dst[3] = (dst >> 32) & 0xFF;
    txData.dst[4] = (dst >> 24) & 0xFF;
    txData.dst[5] = (dst >> 16) & 0xFF;
    txData.dst[6] = (dst >> 8) & 0xFF;
    txData.dst[7] = dst & 0xFF;
    printf("Sending to:");
    printBuff(txData.dst, 8, "0x%x ");
    _xbee_send(data, datalen, radius);
}

uint8_t netdevice_send_hex(char* dst, char* data, uint8_t datalen)
{
    netdevice_send_hex_radius(dst, data, datalen, HOPS_NO_MAX);
}

uint8_t netdevice_send_hex_radius(char* dst, char* data, uint8_t datalen, uint8_t radius)
{
    HAHADEBUG("In xbee_send_hex\n");
    uint8_t len2 = 0, len = 16;// strlen(dst);
    //if(len != 16) return -1; //error

    //Convert the destination to Hex
    len /= 2; /* each byte = 2chars */
    while(len2 < len){
        txData.dst[len2] = asciihex_to_byte(dst[2*len2], dst[2*len2 + 1]);
        len2++;
    }
    HAHADEBUG("sending to:");
    for(int i=0; i<8; ++i)
        HAHADEBUG("%c", txData.dst[i]);
    _xbee_send(data, datalen, radius);
    return 0;
}

uint8_t netdevice_send_byte(char* dst, char* data, uint8_t len)
{
    netdevice_send_byte_radius(dst, data, len, HOPS_NO_MAX);
}

uint8_t netdevice_send_byte_radius(char* dst, char* data, uint8_t len, uint8_t radius)
{
    HAHADEBUG("In xbee_send_hex\n");
    //uint8_t len2 = 0, len = 16;// strlen(dst);
    //if(len != 16) return -1; //error

    //Convert the destination to Hex
    //len /= 2; /* each byte = 2chars */
    //while(len2 < len){
        //txData.dst[len2] = dst[] //asciihex_to_byte(dst[2*len2], dst[2*len2 + 1]);
        //len2++;
    //}
    for(int i=0; i<8; ++i){
        txData.dst[i] = dst[i];
    }
    HAHADEBUG("sending to:");
    for(int i=0; i<8; ++i)
    HAHADEBUG("%c", txData.dst[i]);
    
    return _xbee_send(data, len, radius);
}

/* Internal send function */
uint8_t _xbee_send(char* data, uint8_t len, uint8_t radius)
{
    HAHADEBUG("in _xbee_send\n");
    //Generate the frame packet
    txData.frametype = 0x10;
    txData.frameid = txID++;

    //Reserved: FF FE
    txData.res[0] = 0xFF;
    txData.res[1] = 0xFE;
    txData.b_rad = radius;
    txData.opt = OPT_DIGIMESH;
    txData.data = data;
    txData.data_len = len;

    return _xbee_sendFrame(&txData);
}

uint8_t _frame_TX_tochar(char* buff, frameTX *txData, int size){
    HAHADEBUG("In _frame_TX_tochar\n");
    buff[0] = txData->frametype;
    buff[1] = txData->frameid;
    buff[2] = txData->dst[0];
    buff[3] = txData->dst[1];
    buff[4] = txData->dst[2];
    buff[5] = txData->dst[3];
    buff[6] = txData->dst[4];
    buff[7] = txData->dst[5];
    buff[8] = txData->dst[6];
    buff[9] = txData->dst[7];

    //Network: FF FE
    buff[10] = txData->res[0];
    buff[11] = txData->res[1];
    //buff[10] = 0xFF;
    //buff[11] = 0xFE;

    buff[12] = txData->b_rad;
    buff[13] = txData->opt;
    for(int i=0; i<size; ++i){
        buff[i+FRAME_TX_HEAD_LEN] = txData->data[i];
    }

    return 0;
}

uint8_t _xbee_sendFrame(frameTX *tx){
    HAHADEBUG("_xbee_sendFrame\n");
    /* Convert the data to a char* */
    uint8_t datalen = FRAME_TX_HEAD_LEN+tx->data_len;
    char buffFrameData[datalen];
    _frame_TX_tochar(buffFrameData, tx, tx->data_len);
    tx->checksum = calc_checksum(buffFrameData, datalen);

    /* Gotta split the 16bit int into two 8bits */
    uint8_t len_msb = ((datalen >> 8) & 0xFF);
    uint8_t len_lsb = datalen; //((datalen >> 0) & 0xFF);
    
    /*Send out the UART */
    uart_write(&startdelim, 1);
    uart_write(&len_msb, 1);
    uart_write(&len_lsb, 1);
    uart_write(buffFrameData, datalen);
    uart_write(&tx->checksum, 1);

#ifdef DEBUG_PRINT
    printf("Sent Packet:[");
    printf("%c", startdelim);
    printf("%c", len_msb );
    printf("%c", len_lsb );
    //printf("%c",tx->data_len);
    for(int i=0; i<datalen; ++i)
    printf("%c", buffFrameData[i]);
    printf("%c",tx->checksum);
    printf("]\n");
#endif
    
    //uart_write(&startdelim, 1);
    //uart_write(&zero, 1);
    //uart_write(&tx->framedata_len, 1);
    //uart_write(tx->framedata, tx->framedata_len);
    //uart_write(&tx->checksum, 1);
    return datalen;
};

uint8_t xbee_setAPI(uint8_t type){
    
    xbee_enterCmdMode();
    //Send ATAP 1
    uart_write("ATAP 1\r",7);
    xbee_leaveCmdMode();
    return 0;
}

/* test code
//uint8_t gen_checksum(char* data){
    //return 0;
//}
//
//typedef struct
//{
    //uint8_t 	start;
    //uint16_t 	length;
    //uint8_t 	frameType;  1
    //uint8_t 	src[8];     8
    //uint8_t 	res[2];     2
    //uint8_t 	opt;        1
    //uint8_t		data[100];
    //uint8_t		checksum;
//}rxPacketXbee;
*/

uint8_t netdevice_recv(char* data, uint8_t len){
     printf("netdevice_recv processing %d->%c bytes:[", len, len);
     printBuff(data, len, "%c");
     printf("]\n");
     frameIncoming *incoming = &recvBuff[RXBUFF_CUR];
     static uint8_t state = 0;
     //static uint8_t currsrc = 0;
     //static uint8_t currres = 0;
     static uint8_t currdata = 0;
     uint8_t count = 0;
     HAHADEBUG("\nParsing UART Data\n");
     while(count < len){
         //if(data[count] == startdelim) state = 0;
         switch(state){
             case 0:
                     if(data[count++] == startdelim){
                         HAHADEBUG("Delim:%x\n", data[count-1]);
                         state++;
                         continue;
                     }
                     break;
             case 1:
                     incoming->data_length = data[count++];
                     incoming->data_length = incoming->data_length << 8;
                     HAHADEBUG("LengthMSB incoming:%x data was:%x\n", incoming->data_length, data[count-1]);
                     state++;
                     continue;
                     break;
             case 2:
                     incoming->data_length |= data[count++];
                     incoming->real_length = incoming->data_length - 1;
                     HAHADEBUG("LengthLSB incoming:%x(%d) data was:%x(%d)\n", incoming->data_length,incoming->data_length, data[count-1],data[count-1]);
                     HAHADEBUG("Packet Length minus frametype:%x(%d)\n", incoming->real_length,incoming->real_length);
                     state++;
                     continue;
                     break;
             case 3: /*Technically frametype is part of the 'framedata' but we
                       chop it off for packet processing */
                     incoming->frametype = data[count++];
                     HAHADEBUG("Frame:%x\n", data[count-1]);
                     state++;
                     continue;
                     break;
             case 4: 
                    incoming->data[currdata++] = data[count++];
                    HAHADEBUG("Data[%d]:%c->%c\n",currdata-1,incoming->data[currdata-1], data[count-1]);
                    if(currdata >= incoming->real_length){
                        HAHADEBUG("Data Finished: total:%d\n", incoming->real_length);
                        currdata = 0;
                        state++;
                    }
                    continue;
                    break;
             case 5:
                    incoming->checksum = data[count++];
                    RXBUFF_CUR = !RXBUFF_CUR;
                    state = 0;
                    HAHADEBUG("Chksum:%c->%c\n",incoming->checksum, data[count-1]);
                    HAHADEBUG("netdevice_recv CurrentBuff:%d[%x]->%d[%x]\n", !RXBUFF_CUR,!RXBUFF_CUR, RXBUFF_CUR,RXBUFF_CUR);
                    #ifdef DEBUG_PRINT
                        _print_FrameIncoming(incoming);
                    #endif
                    if(!_verify_checksum(incoming)){
                        _checksum_fail();
                    }                        
                    else
                        _packet_Handler(incoming);
                    incoming = &recvBuff[RXBUFF_CUR];
                    continue;
                    break;
            default:
                    state = 0;
       }
    }  
    return 0;                                       
}    



/************** Internal Functions *******************/
static void _rxFrame_clear(frameRX *p){
    //uint8_t 	start;
    //uint16_t 	length;
    //uint8_t 	frameType;  1
    //uint8_t 	src[8];     8
    //uint8_t 	res[2];     2
    //uint8_t 	opt;        1
    //uint8_t		data[100];
    //uint8_t		checksum;
    //p->data_length = 0;
    p->frametype = 0;

}

/* Will eventually return number of bytes
*  For now it gobbles them up
*/
static uint8_t _get_UART_Data(uint8_t numbytes){
    HAHADEBUG("In _get_UART_Data\n");
    uint8_t ret = 0;
    //SET_RX_CB_OFF; //Future use maybe
    while(ret < numbytes){
        ret = uart_read();
    }
    //SET_RX_CB_ON;
    return ret;
}

//TODO: Flush out remaining types we want to handle
static void _packet_Handler(frameIncoming *f){
    HAHADEBUG("In Packet Handler\n");
    switch(f->frametype){
        case FRAME_MODEM_STATUS: break;
        case FRAME_TX_STATUS:
                    //frameTXStatus *p = &rxData;
                    //printf("TX Status Packet\n");
                    //_parseTXStatus(f, p);
                    //uint8_t id = 
                    printf("TX Status for pid:%d\n");
					printf("TX Status:%x\n", f->data[4]);
                    break;
        case FRAME_ROUTE_INFO: break;
        case FRAME_AGG_ADDR: break;
        case FRAME_RX_EXPLICIT: break;
        case FRAME_DATA_SAMPE: break;
        case FRAME_NODE_ID: break;
        case FRAME_REMOTE_RESP: break;
        case FRAME_AT_RESPONSE:
                    printf("AT CMD![");
                    printBuff(f->data, f->data_length, "%c");
                    printf("]\n");
                    break;
        case FRAME_RX:
                    ;
                    frameRX *p = &rxData;
                    _parseRX(f, p);
                    //uint64_t src = byte_array_to_64(p->src);
                    Network* r = malloc(sizeof(Network));
                    r->src = p->src;
                    r->data = p->data;
                    r->len = p->payload_length;
                    r->id = rxID++;
                    (p->callback)(r);
                    //(p->callback)(p->data, p->payload_length, p->src, rxID++);
                    break;
        default:
                    printf("Got to default\n");
    }
}

/*** Packet Parsers ***/
uint8_t _parseRX(frameIncoming *frame, frameRX *out){
    out->frametype = frame->frametype;
    out->checksum = frame->checksum;
    out->payload_length = frame->data_length - FRAME_RX_HEAD_LEN;
    uint8_t *data = frame->data;
    uint8_t state = 0;
    uint8_t currsrc = 0;
    uint8_t currres = 0;
    uint8_t currdata = 0;
    uint8_t count = 0;
    HAHADEBUG("\nParsing RX Frame Packet\n");
    while(count < frame->real_length){
        switch(state){
           case 0:
                    out->src[currsrc++] = data[count++];
                    HAHADEBUG("Src[%d]:%x->%x\n",currsrc-1,out->src[currsrc-1], data[count-1]);
                    if(currsrc >= 8){
                        HAHADEBUG("SrcEnd:\n");
                        currsrc = 0;
                        state++;
                    }
                    continue;
                    break;
            case 1:
                    out->res[currres++] = data[count++];
                    HAHADEBUG("Res[%d]:%x->%x\n",currres-1,out->res[currres-1], data[count-1]);
                    if(currres >= 2){
                        HAHADEBUG("ResEnd:\n");
                        currres = 0;
                        state++;
                    }
                    continue;
                    break;
            case 2:
                    out->opt = data[count++];
                    HAHADEBUG("Opt:%x\n", data[count-1]);
                    state++;
                    continue;
                    break;
            case 3:
                    out->data[currdata++] = data[count++];
                    HAHADEBUG("Data[%d]:%c %c\n",currdata-1,out->data[currdata-1], data[count-1]);
                    if(currdata >= out->payload_length){
                        HAHADEBUG("Data Finished: total:%d\n", out->payload_length);
                        currdata = 0;
                        state++;
                    }
                    continue;
                    break;
            default:
                    state = 0;
        }
    }
    
    return 0;
}

/* Prints a RX Frame Packet */
static void _print_Packet(frameRX *p){
    printf("\n\n**rxPacketXbee Print**\n");
    printf("payload length:%x\nframeType:%x\n",
    p->payload_length,
    p->frametype
    );
    printf("source:");
    for(int i=0; i<8; ++i)
    printf("%x",p->src[i]);
    printf("\n");
    printf("options:%x\n", p->opt);
    char buff[101];
    sprintf(buff, p->data, 100);
    buff[101] = '\0';
    printf("data:[");
    printBuff(p->data, p->payload_length,"%c");
    printf("]");
    printf("checksum:%x\n", p->checksum);
}

/* Prints a RX Frame Packet */
static void _print_FrameIncoming(frameIncoming *p){
    printf("\n\n**FrameIncoming Print**\n");
    printf("length:%x\nframeType:%x\n",
    p->data_length,
    p->frametype
    );
    printf("Actual length of datafield:%x\n", p->real_length);
    printf("data:[");
    printBuff(p->data, p->real_length, "%c");
    printf("]\n");
    printf("checksum:%x\n", p->checksum);
}

/* Verify incoming frame Checksum */
uint8_t _verify_checksum(frameIncoming *f){
    HAHADEBUG("Verifying checksum\n");
    uint8_t sum = f->frametype;
    for(int i=0; i<f->real_length; ++i)
        sum += f->data[i];
    sum += f->checksum;
    HAHADEBUG("Checksum:%x\n", sum);
    return sum == 0xFF;
}

/* Handle checksum failures */
static void _checksum_fail(){
    HAHADEBUG("**Checksum Failed!**\n");
}                            
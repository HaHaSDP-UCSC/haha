/*
* xbee.c
*
* Created: 3/5/2017 6:03:43 PM
*  Author: Brian
*/
#include "xbee/xbee.h"
#include "devstuff/uart.h"
#include <string.h>
#include "devstuff/hahaUtils.h"


/* Internal Functions */
static void _rxFrame_clear(frameRX *p);
static uint8_t _xbee_send(char* data, uint8_t len);
static uint8_t _frame_TX_tochar(char* buff, frameTX *txData, int size);
static void _packet_Handler(void* frame, uint8_t type);
static uint8_t _get_UART_Data(uint8_t numbytes);
static void _print_Packet(frameRX *p);


/* Frame data */
const uint8_t startdelim = 0x7E;
frameRX recvBuff[2];
frameTX txData;
uint8_t RXBUFF_CUR = 0;

/* Frames to XBee */
typedef enum frameRequestType {	FRAME_AT = 0x08,
                                FRAME_AT_QUEUE = 0x09,
                                FRAME_TX = 0x10, 
                                FRAME_TX_EXPLICIT = 0x11, 
                                FRAME_REMOTE_COMMAND = 0x17
                              };
/* Frames from XBee */
typedef enum frameResponseType{	FRAME_AT_RESPONSE = 0x88,
                                FRAME_MODEM_STATUS = 0x8A,
                                FRAME_TX_STATUS = 0x8B,
                                FRAME_ROUTE_INFO = 0x8D,
                                FRAME_AGG_ADDR = 0x8E,
                                FRAME_RX = 0x90, //only for AO=0
                                FRAME_RX_EXPLICIT = 0x91, //only for AO=1
                                FRAME_DATA_SAMPE = 0x92,
                                FRAME_NODE_ID = 0x95,
                                FRAME_REMOTE_RESP = 0x97								
                                };


void xbee_init(){
    _rxFrame_clear(&recvBuff[0]);
    _rxFrame_clear(&recvBuff[1]);
}

void xbee_register_callback(xbee_cb_t t){
    recvBuff[0].callback = t;
    recvBuff[1].callback = t;
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

/**
 * [calc_checksum description]
 * @param  data [description]
 * @param  len  [description]
 * @return      [description]
 */
uint8_t calc_checksum(char *data, uint8_t len){
    HAHADEBUG("Calculating checksum\n");
    uint16_t total = 0;
    for(int i=0; i<len; ++i){
        total+= data[i];
    }
    return 0xFF - total;
}

/* Exposed send function */
uint8_t xbee_send(char* dst, char* data, uint8_t datalen)
{
    HAHADEBUG("In xbee_send\n");
    uint8_t len2 = 0, len = strlen(dst);
    if(len != 16) return -1; //error

    //Convert the destination to Hex
    len /= 2; /* each byte = 2chars */
    while(len2 < len){
        txData.dst[len2] = asciihex_to_byte(dst[2*len2], dst[2*len2 + 1]);
        len2++;
    }

    _xbee_send(data, datalen);
}

/* Internal send function */
uint8_t _xbee_send(char* data, uint8_t len)
{
    HAHADEBUG("in _xbee_send\n");
    //Generate the frame packet
    txData.frametype = 0x10;
    txData.frameid = 0x55; //TODO:generate this later

    //Reserved: FF FE
    txData.res[0] = 0xFF;
    txData.res[1] = 0xFE;
    txData.b_rad = 0;
    txData.opt = OPT_DIGIMESH;
    txData.data = data;
    txData.data_len = len;

    _xbee_sendFrame(&txData);

    return 0;
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

void _xbee_sendFrame(frameTX *tx){
    HAHADEBUG("_xbee_sendFrame\n");
    /* Convert the data to a char* */
    uint8_t datalen = FRAME_TX_HEAD_LEN+tx->data_len;
    char buffFrameData[datalen];
    _frame_TX_tochar(buffFrameData, tx, tx->data_len);
    tx->checksum = calc_checksum(buffFrameData, datalen);

    /* Gotta split the 16bit int into two 8bits */
    uint8_t len_msb = ((datalen >> 8) & 0xFF);
    uint8_t len_lsb = ((datalen >> 0) & 0xFF);
    
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
    printf("%c",tx->data_len);
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

uint8_t xbee_recv(char* data, uint8_t len){
    //Need to process packets
    printf("xbee_recv processing %d->%c bytes:[", len, len);
    printBuff(data, len, "%c");
    printf("]");
    frameRX *incoming = &recvBuff[RXBUFF_CUR];
    static uint8_t state = 0;
    static uint8_t currsrc = 0;
    static uint8_t currres = 0;
    static uint8_t currdata = 0;
    uint8_t count = 0;
    HAHADEBUG("\nParsing UART Data\n");
    while(count < len){
        //HAHADEBUG("Running While Loop count:%d len:%d data:%c\n",count, len,incoming->data[count]);
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
                    incoming->data_length << 8;
                    HAHADEBUG("LengthMSB incoming:%x data was:%x\n", incoming->data_length, data[count-1]);
                    state++;
                    continue;
                    break;
            case 2: 
                    incoming->data_length |= data[count++];
                    incoming->payload_length = incoming->data_length - FRAME_RX_HEAD_LEN;
                    HAHADEBUG("LengthLSB incoming:%x data was:%x\n", incoming->data_length, data[count-1]);
                    HAHADEBUG("Computed Data Packet Length:%d\n", incoming->payload_length);
                    state++;
                    continue;
                    break;
            case 3:
                    incoming->frametype = data[count++];
                    HAHADEBUG("Frame:%x\n", data[count-1]);
                    if(incoming->frametype != 0x90){ state=0; continue;}
                    state++;
                    continue;
                    break;
            case 4:
                    incoming->src[currsrc++] = data[count++];
                    HAHADEBUG("Src[%d]:%x->%x\n",currsrc-1,incoming->src[currsrc-1], data[count-1]);
                    if(currsrc >= 8){ 
                        HAHADEBUG("ResEnd:\n");
                        currsrc = 0; 
                        state++; 
                    }
                    continue;
                    break;
            case 5:
                    incoming->res[currres++] = data[count++];
                    HAHADEBUG("Res[%d]:%x->%x\n",currres-1,incoming->res[currres-1], data[count-1]);
                    if(currres >= 2){ 
                        HAHADEBUG("SrcEnd:\n");
                        currres = 0; 
                        state++; 
                }
                    continue;
                    break;
            case 6:
                    incoming->opt = data[count++];
                    HAHADEBUG("Opt:%x\n", data[count-1]);
                    state++;
                    continue;
                    break;
            case 7:
                    incoming->data[currdata++] = data[count++];
                    HAHADEBUG("Data[%d]:%c %c\n",currdata-1,incoming->data[currdata-1], data[count-1]);
                    if(currdata >= incoming->payload_length){ 
                        HAHADEBUG("Data Finished: total:%d\n", incoming->payload_length);
                        currdata = 0; 
                        state++;
                    }
                    continue;
                    break;
            case 8:
                    incoming->checksum = data[count++];
                    HAHADEBUG("Chksum:%c %c\n",incoming->checksum, data[count-1]);
                    state = 0;
                    #ifdef DEBUG_PRINT
                        //printPacket(incoming);
                    #endif
                    /* Handle Packet */
                    _packet_Handler(incoming, incoming->frametype);
                    incoming->payload_length = 0;
                    RXBUFF_CUR = !RXBUFF_CUR;
                    printf("xBeePacket CurrentBuff:%d[%x]->%d[%x]\n", !RXBUFF_CUR,!RXBUFF_CUR, RXBUFF_CUR,RXBUFF_CUR);
                    incoming = &recvBuff[RXBUFF_CUR];
                    continue;
                    break;
            default:
                    state = 0;
        }
    }
}

/*Interal Functions */
static _rxFrame_clear(frameRX *p){
    //uint8_t 	start;
    //uint16_t 	length;
    //uint8_t 	frameType;  1
    //uint8_t 	src[8];     8
    //uint8_t 	res[2];     2
    //uint8_t 	opt;        1
    //uint8_t		data[100];
    //uint8_t		checksum;
    p->data_length = 0;
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

static void _packet_Handler(void* frame, uint8_t type){
    HAHADEBUG("In Packet Handler\n");
    switch(type){
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
                    ;
                    frameRX *p = (frameRX*)frame;
                    //printPacket(p);
                    (p->callback)(p->data,p->payload_length);
                    break;
        default:
                    printf("Got to default\n");
    }
}

/* Prints a RX Frame Packet */
static void _print_Packet(frameRX *p){
    printf("\n\n**rxPacketXbee Print**\n");
    printf("length:%x\nframeType:%x\n",
    p->data_length,
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
    printBuff(p->data, p->data_length,"%c");
    printf("]");
    printf("checksum:%x\n", p->checksum);
}






/*************************** OLD STUFF ******************** */
/*
//~~~Frames~~~
//typedef struct {
//uint8_t length;
//uint8_t frametype;
//void	*framedata;
//uint8_t framedata_len;
//uint8_t checksum;
//}Frame;


*/
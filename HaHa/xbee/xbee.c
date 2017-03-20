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
void _rxFrame_clear(frameRX *p);
uint8_t _xbee_send(char* data, uint8_t len);
uint8_t _frame_TX_tochar(char* buff, frameTX *txData, int size);



const uint8_t startdelim = 0x7E;



frameRX recvBuff[2];
frameTX txData1;

uint8_t RXBUFF_CUR = 0;


const char get_own_mac_low[] = 			"7E00040852534C06"; 	// AT+SL
const char get_own_mac_high[]= 			"7E0004085253480A"; 	// AT+SH
const char set_own_net_address[]= 		"7E000608524D59000000";	// AT+MY
const char get_own_net_address[]= 		"7E000408524D59FF";		// AT+MY
const char set_api_mode[] = 			"7E0005085241500113";	// AT+AP
const char* const table_CORE[] =
{
	get_own_mac_low, 		// 0
	get_own_mac_high,		// 1
	set_own_net_address,	// 2
	get_own_net_address,	// 3
	set_api_mode,
};

void xbee_init(){
	_rxFrame_clear(&recvBuff[0]);
	_rxFrame_clear(&recvBuff[1]);
}

void xbee_enterCmdMode(){
	//Send '+++' to enter command mode guarded by 1 sec on each side
	printf("Configuring Xbee for API mode...\n");
	delay(1000);
	uart_write("+++", 3);
	delay(1000);
	//Make sure returned 'OK<cr>'
	uint8_t ret = 0;
	SET_RX_CB_OFF;
	while(ret < 3){
		ret = uart_read();
	}
	//Clear the buffered message
	uart_clearbuffer();
	SET_RX_CB_ON;
}

void xbee_leaveCmdMode(){
	printf("Xbee leaving command mode...\n");
	SET_RX_CB_OFF;
	uart_write("ATCN\r", 5);
	SET_RX_CB_ON;
}

/**
 * [calc_checksum description]
 * @param  data [description]
 * @param  len  [description]
 * @return      [description]
 */
uint8_t calc_checksum(char *data, uint8_t len){
	uint16_t total = 0;
	for(int i=0; i<len; ++i){
		total+= data[i];
	}
	printf("calcchecksum:%cEND", total);
	return 0xFF - total;
}

/* Exposed send function */
/* Assumes data is null-terminated */
//TODO: This should be modified to take a size argument
uint8_t xbee_send(char* dst, char* data)
{
	uint8_t len2 = 0, len = strlen(dst);
	if(len != 16) return -1; //error

	//Convert the destination to Hex
	len /= 2; /* each byte = 2chars */
	while(len2 < len){
		txData1.dst[len2] = asciihex_to_byte(dst[2*len2], dst[2*len2 + 1]);
		len2++;
	}

	_xbee_send(data, strlen(data));
}

/* Internal send function */
uint8_t _xbee_send(char* data, uint8_t len)
{
	//Generate the frame packet
	txData1.frametype = 0x10;
	txData1.frameid = 0x55; //TODO:generate this later

	//Reserved: FF FE
	txData1.res[0] = 0xFF;
	txData1.res[1] = 0xFE;
	txData1.b_rad = 0;
	txData1.opt = OPT_DIGIMESH;
	txData1.data = data;
	txData1.data_len = len;

	_xbee_sendFrame(&txData1);

	return 0;
}

uint8_t _frame_TX_tochar(char* buff, frameTX *txData, int size){
	printf("inTXChar:");
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
	/* Convert the data to a char* */
	uint8_t datalen = FRAME_TX_HEAD_LEN+tx->data_len;
	printf("buffsize:%d\n",FRAME_TX_HEAD_LEN+tx->data_len);
	char buffFrameData[datalen];
	_frame_TX_tochar(buffFrameData, tx, tx->data_len);
	tx->checksum = calc_checksum(buffFrameData, datalen);
	printf("checksum:%d", tx->checksum);
	printf("printing buffer:");
	printBuff(buffFrameData, datalen, "%c");
	printf("END");
	//tx.framedata = buffFrameData;
	//tx.framedata_len = FRAME_TX_HEAD_LEN + txData.data_len;

	/* Gotta split the 16bit int into two 8bits */
	uint8_t len_msb = ((datalen >> 8) & 0xFF);
	uint8_t len_lsb = ((datalen >> 0) & 0xFF);
	printf("len:%x",datalen);
	printf("lenhi:%x", len_msb);
	printf("lenlo:%x", len_lsb);
	uart_write(&startdelim, 1);
	uart_write(&len_msb, 1);
	uart_write(&len_lsb, 1);
	//uart_write(&tx->frametype, 1);
	uart_write(buffFrameData, datalen);
	uart_write(&tx->checksum, 1);
	
	printf("%c", startdelim);
	printf("%c", len_msb );
	printf("%c", len_lsb );
	printf("%c",tx->data_len);
	//printf("%c",tx->frametype);
	for(int i=0; i<datalen; ++i)
	printf("%c", buffFrameData[i]);
	printf("%c",tx->checksum);
	
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

void printPacket(frameRX *p){
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
	printf("data:%sDATAEND\n", buff);
	printf("checksum:%x\n", p->checksum);
}

uint8_t xbee_recv(char* data, uint8_t len){
	//Need to process packets
	frameRX *incoming = &recvBuff[RXBUFF_CUR];
	static uint8_t state = 0;
	static uint8_t currsrc = 0;
	static uint8_t currres = 0;
	static uint8_t currdata = 0;
	static uint8_t datalen = 0;
	uint8_t count = 0;
	DPRINTF("Parsing UART Data\n");
	while(count < len){
		//DPRINTF("Running While Loop count:%d len:%d data:%c\n",count, len,incoming->data[count]);
		switch(state){
			case 0:
					if(data[count++] == startdelim){
						DPRINTF("Delim:%x\n", data[count-1]);
						//incoming->start = data[0];
						state++;
						continue;
					}
					break;
			case 1:
					incoming->data_length = data[count++] << 8;
					DPRINTF("LengthMSB incoming:%x data:%x\n", incoming->data_length, data[count-1]);
					state++;
					continue;
					break;
			case 2: 
					incoming->data_length |= data[count++];
					datalen = incoming->data_length - 12; //12=other fields #of data
					DPRINTF("LengthLSB incoming:%x data:%x\n", incoming->data_length, data[count-1]);
					state++;
					continue;
					break;
			case 3:
					incoming->frametype = data[count++];
					DPRINTF("Frame:%x\n", data[count-1]);
					if(incoming->frametype != 0x90){ state=0; continue;}
					state++;
					continue;
					break;
			case 4:
					incoming->src[currsrc++] = data[count++];
					DPRINTF("Src[%d]:%x->%x\n",currsrc-1,incoming->src[currsrc-1], data[count-1]);
					if(currsrc >= 8){ 
						DPRINTF("ResEnd:\n");
						currsrc = 0; 
						state++; 
					}
					continue;
					break;
			case 5:
					incoming->res[currres++] = data[count++];
					DPRINTF("Res[%d]:%x->%x\n",currres-1,incoming->res[currres-1], data[count-1]);
					if(currres >= 2){ 
						DPRINTF("SrcEnd:\n");
						currres = 0; 
						state++; 
				}
					continue;
					break;
			case 6:
					incoming->opt = data[count++];
					DPRINTF("Opt:%x\n", data[count-1]);
					state++;
					continue;
					break;
			case 7:
					incoming->data[currdata++] = data[count++];
					DPRINTF("Data[%d]:%c %c\n",currdata-1,incoming->data[currdata-1], data[count-1]);
					if(currdata >= datalen){ 
						DPRINTF("Data Finished: total:%d\n", datalen);
						datalen= 0; 
						state++;
					}
					continue;
					break;
			case 8:
					incoming->checksum = data[count++];
					DPRINTF("Chksum:%c %c\n",incoming->checksum, data[count-1]);
					state = 0;
					RXBUFF_CUR = !RXBUFF_CUR;
					printPacket(incoming);
					continue;
					break;
			default:
					state = 0;
		}
	}
}

/*Interal Functions */
_rxFrame_clear(frameRX *p){
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
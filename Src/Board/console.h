/*
 * console.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_BOARD_CONSOLE_H_
#define SRC_BOARD_CONSOLE_H_

#include "..\inc\type.h"

#define MAX_DATA_LEN	32
#define MAX_FRAME_LEN	(MAX_DATA_LEN+1) 	//data + crc
#define MIN_FRAME_LEN	2					//crc + 1data

typedef struct {
	uint8 rtype;		//收到的帧类型
	uint8 rlen;			//收到的长度
	uint8 rdata[MAX_DATA_LEN];	//收到的数据
	uint8 ttype;
	uint8 tlen;
	uint8 tdata[MAX_DATA_LEN];

}CMD_FRAME;

void initConsole(void);

#endif /* SRC_BOARD_CONSOLE_H_ */

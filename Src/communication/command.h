/*
 * command.h
 *
 *  Created on: 2017年7月29日
 *      Author: Administrator
 */

#ifndef SRC_COMMUNICATION_COMMAND_H_
#define SRC_COMMUNICATION_COMMAND_H_

#include "..\inc\type.h"

#define MAX_DATA_LEN	32
#define MAX_LEN_VALUE	(MAX_DATA_LEN+1) 	//长度字节最大值   32字节数据+1字节CRC
#define MIN_LEN_VALUE	2					//长度字节最小值，1字节数据+1字节CRC
#define MAX_FRAME_LEN	(MAX_LEN_VALUE + 3) //包括7E在内的帧总字节数
#define MIN_FRAME_LEN	(MIN_LEN_VALUE + 3)

#define CMD_OK		0x5A
#define CMD_ERR		0xEE
#define CRC_FIX		0x5A

#define FRAME_HEAD	0x7E
#define TRANSLATE_HEAD	0x7D
#define TRANSLATE_TAIL1	0x5E
#define TRANSLATE_TAIL2	0x5D


enum {
	LOCAL_CMD = 0,
	REMOTE_CMD = 1,
	LOOP_DETEC = 2,
};

enum {
	CMD_FACTORY_INFO 	= 1,
	CMD_SET_FACTORY		= 2,
	CMD_LED_STA			= 3,
	CMD_ALM_SIG_GET		= 4,
	CMD_ALM_ATTR_GET	= 5,
	CMD_ALM_ATTR_SET	= 6,
	CMD_VCG_CFG_GET		= 7,
	CMD_VCG_CFG_SET		= 8,
	CMD_DEBUG			= 0xdb,
};

typedef struct {
	uint8 rtype;		//收到的帧类型
	uint8 rlen;			//收到的长度
	uint8 rdata[MAX_DATA_LEN];	//收到的数据
	uint8 ttype;
	uint8 tlen;
	uint8 tdata[MAX_DATA_LEN];

}CMD_FRAME;

void processCMD(CMD_FRAME* f);

#endif /* SRC_COMMUNICATION_COMMAND_H_ */

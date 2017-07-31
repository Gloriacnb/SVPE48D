/*
 * command.c
 *
 *  Created on: 2017年7月29日
 *      Author: Administrator
 */

#ifndef SRC_COMMUNICATION_COMMAND_C_
#define SRC_COMMUNICATION_COMMAND_C_

#include "..\communication\command.h"
#include "..\Business\Device.h"

#include "../inc/ErrCode.h"

static bool checkCRC(CMD_FRAME* f);
static uint8 calculateCRC8(uint8* d, uint8 len);
static void nack(CMD_FRAME* f, uint8 errcode);
static void ack(CMD_FRAME* f);
void processCMD(CMD_FRAME* f) {
	uint8 bytes = 0;
	uint8 errcode = 0;
	if( !checkCRC(f) ) {
		nack(f, ERR_CRC);	//校验失败，返回CRC错误
	}
	else {
		f->tdata[0] = f->rdata[0]; //维持原命令字节
		switch(f->rdata[0]) {
		case CMD_FACTORY_INFO:
			bytes = getFactoryInfo(&(f->tdata[2]));
			f->tdata[1] = CMD_OK;
			f->tlen = bytes + 2; 			//data length: NM + sta + info
			f->tdata[f->tlen++] = CRC_FIX;	//增加CRC字节，长度+1
			break;
		case CMD_SET_FACTORY:
			errcode = setFactoryInfo(&f->rdata[1], f->rlen-2);
			if( errcode == ERR_NONE ) {
				ack(f);
			}
			else {
				nack(f, errcode);
			}
			break;
		default:
			nack(f, ERR_NOSUCHNM);
			break;
		}
	}
}


bool checkCRC(CMD_FRAME* f){
	if( f->rdata[f->rlen-1] == CRC_FIX ) {
		return true;
	}
	return false;
}

uint8 calculateCRC8(uint8* d, uint8 len) {
    d[len] = CRC_FIX;
	return CRC_FIX;
}

void nack(CMD_FRAME* f, uint8 errcode) {
	f->tlen = 4;
	f->tdata[0] = f->rdata[0]; //维持原命令字节
	f->tdata[1] = CMD_ERR;
	f->tdata[2] = errcode;
	f->tdata[3] = CRC_FIX;			//CRC fix

}
void ack(CMD_FRAME* f) {
	f->tlen = 3;
	f->tdata[0] = f->rdata[0]; //维持原命令字节
	f->tdata[1] = CMD_OK;
	f->tdata[2] = CRC_FIX;			//CRC fix

}
#endif /* SRC_COMMUNICATION_COMMAND_C_ */

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
#include "../Board/ChipSE0165B.h"

#include "../inc/ErrCode.h"
#include "../Alarm/AlarmModule.h"
#include "../Business/VCG.h"

extern uint8 bdata LED_STA;

static bool checkCRC(CMD_FRAME* f);
static uint8 calculateCRC8(uint8* d, uint8 len);
static void nack(CMD_FRAME* f, uint8 errcode);
static void ack(CMD_FRAME* f);
static void padTData(CMD_FRAME* f, uint8 tlen);//填充返回值

void processCMD(CMD_FRAME* f) {
	uint8 i = 0;
	uint8 oParaBytes = 0;			//返回数据字节数
	uint8 errcode = 0;
	uint8* oPara = 0;
	uint8* iPara = 0;
	uint8 iParaBytes = 0;
	ALM_SRC* s = 0;

	if( !checkCRC(f) ) {
		nack(f, ERR_CRC);	//校验失败，返回CRC错误
	}
	else {
		f->tdata[0] = f->rdata[0]; 	//维持原命令字节
		iPara = &(f->rdata[1]);		//接收参数列表首地址
		iParaBytes = f->rlen-2;		//接收参数字节数
		oPara = &(f->tdata[2]);		//返回数据首地址
		switch(f->rdata[0]) {
		case CMD_FACTORY_INFO:
			oParaBytes = getFactoryInfo(oPara);
			padTData(f, oParaBytes);
			break;
		case CMD_SET_FACTORY:
			errcode = setFactoryInfo(iPara, iParaBytes);
			if( errcode == ERR_NONE ) {
				ack(f);
			}
			else {
				nack(f, errcode);
			}
			break;
		case CMD_LED_STA:
			*oPara = LED_STA;
			padTData(f, 1);
			break;
		case CMD_ALM_ATTR_GET:
			if( iPara[1] == 0 ) {//读取所有告警类型
				s = getAlarmSourceBySid(iPara[0]);	//获取告警源
				if(s) {
					/*读取该告警源下所有告警类型的属性值*/
					for (i = 0; i < s->typeCount; ++i) {
						oPara[i] = getAlarmAttribute(iPara[0], s->type[i].tid);
					}
					padTData(f, s->typeCount);
				}
				else {
					/*告警源为空，返回错误*/
					nack(f, ERR_NULLPOINTER);
				}
			}
			else {//读取指定的一个属性
				*oPara = getAlarmAttribute(iPara[0], iPara[1]);
				padTData(f, 1);
			}
			break;
		case CMD_ALM_ATTR_SET:
			if( iPara[1] == 0 ) {
				s = getAlarmSourceBySid(iPara[0]);	//获取告警源
				if(s) {
					for (i = 0; i < s->typeCount; ++i) {
						setAlarmAttribute(iPara[0], s->type[i].tid, iPara[2]);
					}
					ack(f);
				}
				else {
					/*告警源为空，返回错误*/
					nack(f, ERR_NULLPOINTER);
				}
			}
			else {
				if( setAlarmAttribute(iPara[0], iPara[1], iPara[2]) ) {
					ack(f);
				}
				else {
					nack(f, ERR_SETFAILED);
				}
			}
			break;
		case CMD_ALM_SIG_GET:
			s = getAlarmSourceBySid(iPara[0]);	//获取告警源
			if( s ) {
				*oPara = s->type[SN(iPara[1])].actState?1:0;
				padTData(f, 1);
			}
			else {
				/*告警源为空，返回错误*/
				nack(f, ERR_NULLPOINTER);
			}
			break;
		case CMD_VCG_CFG_GET:
			padTData(f, getVCGConfigData(oPara));
			break;
		case CMD_VCG_CFG_SET:
			if( setVCGConfig(iPara, iParaBytes) ) {
				ack(f);
			}
			else {
				nack(f, ERR_SETFAILED);
			}
			break;
		case CMD_DEBUG:
			if( f->rlen >= 4 ) {
				switch(f->rdata[1]) {
				case 1: //read se0165B register
					*oPara = readSE0165B( (iPara[1]<<8) | iPara[2] );
					padTData(f, 1);
					break;
				case 2: //write se0165B register
					writeSE0165B( (iPara[1]<<8) | iPara[2], iPara[3] );
					ack(f);
					break;
				default:
					nack(f, ERR_NOSUCHNM);
					break;
				}

			}
			else {
				nack(f, ERR_INPUT);
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

void padTData(CMD_FRAME* f, uint8 tlen) {
	f->tdata[1] = CMD_OK;
	f->tlen = tlen + 2; 			//data length: NM + rsta + data
	f->tdata[f->tlen++] = CRC_FIX;	//增加CRC字节，长度+1
}
#endif /* SRC_COMMUNICATION_COMMAND_C_ */

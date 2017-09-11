/*
 * AlmSrcE1.c
 *
 *  Created on: 2017年9月5日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcE1.h"
#include "../inc/ErrCode.h"
#include "../Business/ConfigData.h"
#include "../inc/taskID.h"
#include "../STCLib/USART.h"

static bool ifE1Alarm(uint8 sid, ALM_TYPE* types);
static bool saveE1Cfg(void);
static xdata E1_CFG_DATA CFG_DATA[E1_ALM_SRC_N] = {0};
extern int xdata E1_ALARM[4];

int initALMSrcE1(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	readE1ConfigData();
	for (i = 0; i < count; ++i) {
		src[i].sid = E1_ASID_BASE + i;
		src[i].saveCfg = saveE1Cfg;
		src[i].typeCount = E1_TYPE_SIZE;
		src[i].getAlarm = ifE1Alarm;
		for (j = 0; j < E1_TYPE_SIZE; ++j) {
			src[i].type[j].tid = E1_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = CFG_DATA[i].attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

bool ifE1Alarm(uint8 sid, ALM_TYPE* types) {
	int alarmd;
	uint8 i;
	alarmd = E1_ALARM[SN(sid)];	//此处不直接读E1状态寄存器，点灯时已经读取了，信号会读清，不能频繁读
	for (i = 0; i < E1_TYPE_SIZE-1; ++i) {
		types[i].actState = ((alarmd & (1<<i)) != 0);
	}
	types[i].actState = ((alarmd & (1<<7)) != 0);
	return true;
}

void readE1ConfigData(void) {
	uint8 i,j;
	readConfig(E1_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(E1_CFG_DATA)*E1_ALM_SRC_N);
	if( CFG_DATA[0].attr[0] == 0xff ) {
		for (i = 0; i < E1_ALM_SRC_N; ++i) {
			for (j = 0; j < E1_TYPE_SIZE; ++j) {
				CFG_DATA[i].attr[j] = 0x0A;
			}
		}
		saveE1Cfg();
	}
}
bool saveE1Cfg(void) {
	saveConfig(E1_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(E1_CFG_DATA)*E1_ALM_SRC_N);
	return true;
}

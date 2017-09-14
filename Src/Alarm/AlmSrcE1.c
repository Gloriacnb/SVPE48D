/*
 * AlmSrcE1.c
 *
 *  Created on: 2017年9月5日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcE1.h"
#include "../inc/ErrCode.h"
#include "../Business/E1Port.h"

static bool ifE1Alarm(uint8 sid, ALM_TYPE* types);
extern int xdata E1_ALARM[4];

int initALMSrcE1(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	for (i = 0; i < count; ++i) {
		src[i].sid = E1_ASID_BASE + i;
		src[i].saveCfg = saveE1Cfg;
		src[i].typeCount = E1_TYPE_SIZE;
		src[i].getAlarm = ifE1Alarm;
		for (j = 0; j < E1_TYPE_SIZE; ++j) {
			src[i].type[j].tid = E1_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = getE1ConfigStruct(i)->attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

/*
 * 告警处理任务的回调函数，负责采集各个告警信号的最新状态
 */
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


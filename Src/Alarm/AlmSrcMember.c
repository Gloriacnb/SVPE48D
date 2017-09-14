/*
 * AlmSrcMember.c
 *
 *  Created on: 2017年9月11日
 *      Author: Administrator
 */


#include "../Alarm/AlmSrcMember.h"
#include "../inc/ErrCode.h"
#include "../RTLLib/rtk_api_ext.h"
#include "../Board/ChipSE0165B.h"
#include "../Business/Member.h"

static bool ifMemberAlarm(uint8 sid, ALM_TYPE* types);

int initALMSrcMEM(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	for (i = 0; i < count; ++i) {
		src[i].sid = MEM_ASID_BASE + i;
		src[i].saveCfg = saveMemberCfg;
		src[i].typeCount = MEM_TYPE_SIZE;
		src[i].getAlarm = ifMemberAlarm;
		for (j = 0; j < MEM_TYPE_SIZE; ++j) {
			src[i].type[j].tid = MEM_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = getMembConfigStruct(i)->attr[j];	//恢复配置
		}
	}
	return ERR_NONE;
}

/*
 * 告警处理任务的回调函数，负责采集各个告警信号的最新状态
 */
bool ifMemberAlarm(uint8 sid, ALM_TYPE* types) {
	uint8 i = 0;
	uint8 alarmd = getMemberAlarm(SN(sid));
	if( types == 0 ) {
		return false;
	}
	for (i = 0; i < MEM_TYPE_SIZE; ++i) {
		types[i].actState = ((alarmd & (1<<i)) != 0);
	}
	return true;
}


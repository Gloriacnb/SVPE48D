/*
 * AlmSrcVCG.c
 *
 *  Created on: 2017年9月8日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcVCG.h"
#include "../inc/ErrCode.h"
#include "../Board/ChipSE0165B.h"
#include "../Business/VCG.h"

static bool ifVCGAlarm(uint8 sid, ALM_TYPE* types);

int initALMSrcVCG(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 || count != 1) {
		return ERR_INPUT;
	}
	for (i = 0; i < count; ++i) {
		src[i].sid = VCG_ASID_BASE + i;
		src[i].saveCfg = saveVCGCfg;
		src[i].typeCount = VCG_TYPE_SIZE;
		src[i].getAlarm = ifVCGAlarm;
		for (j = 0; j < VCG_TYPE_SIZE; ++j) {
			src[i].type[j].tid = VCG_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = getVCGConfigStruct()->attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

/*
 * 告警处理任务的回调函数，负责采集各个告警信号的最新状态
 */
bool ifVCGAlarm(uint8 sid, ALM_TYPE* types) {
	uint8 i = 0;
	uint8 vcgalm = getVCGAlarm();
	if( types == 0 || sid != VCG_ASID_BASE) {
		return false;
	}
	for (i = 0; i < VCG_TYPE_SIZE; ++i) {
		types[i].actState = ((vcgalm & (1<<i)) != 0);
	}

	return true;
}


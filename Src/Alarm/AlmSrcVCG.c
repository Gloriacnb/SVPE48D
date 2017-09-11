/*
 * AlmSrcVCG.c
 *
 *  Created on: 2017年9月8日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcVCG.h"
#include "../inc/ErrCode.h"
#include "../Business/ConfigData.h"
#include "../inc/taskID.h"
#include "../Board/ChipSE0165B.h"

static bool ifVCGAlarm(uint8 sid, ALM_TYPE* types);
static bool saveVCGCfg(void);
static xdata VCG_CFG_DATA CFG_DATA[VCG_ALM_SRC_N] = {0};

int initALMSrcVCG(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 || count != 1) {
		return ERR_INPUT;
	}
	readVCGConfigData();
	for (i = 0; i < count; ++i) {
		src[i].sid = VCG_ASID_BASE + i;
		src[i].saveCfg = saveVCGCfg;
		src[i].typeCount = VCG_TYPE_SIZE;
		src[i].getAlarm = ifVCGAlarm;
		for (j = 0; j < VCG_TYPE_SIZE; ++j) {
			src[i].type[j].tid = VCG_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = CFG_DATA[i].attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

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

void readVCGConfigData(void) {
	uint8 i,j;
	readConfig(VCG_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(VCG_CFG_DATA)*VCG_ALM_SRC_N);
	if( CFG_DATA[0].attr[0] == 0xff ) {
		for (i = 0; i < VCG_ALM_SRC_N; ++i) {
			for (j = 0; j < VCG_TYPE_SIZE; ++j) {
				CFG_DATA[i].attr[j] = 0x0A;
			}
		}
		saveVCGCfg();
	}
}
bool saveVCGCfg(void) {
	saveConfig(VCG_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(VCG_CFG_DATA)*VCG_ALM_SRC_N);
	return true;
}

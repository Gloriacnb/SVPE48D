/*
 * AlmSrcMember.c
 *
 *  Created on: 2017年9月11日
 *      Author: Administrator
 */


#include "../Alarm/AlmSrcMember.h"
#include "../inc/ErrCode.h"
#include "../Business/ConfigData.h"
#include "../inc/taskID.h"
#include "../RTLLib/rtk_api_ext.h"
#include "../Board/ChipSE0165B.h"

static bool ifMemberAlarm(uint8 sid, ALM_TYPE* types);
static bool saveMemberCfg(void);
static xdata MEM_CFG_DATA CFG_DATA[MEM_ALM_SRC_N] = {0};

int initALMSrcMEM(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	readMemberConfigData();
	for (i = 0; i < count; ++i) {
		src[i].sid = MEM_ASID_BASE + i;
		src[i].saveCfg = saveMemberCfg;
		src[i].typeCount = MEM_TYPE_SIZE;
		src[i].getAlarm = ifMemberAlarm;
		for (j = 0; j < MEM_TYPE_SIZE; ++j) {
			src[i].type[j].tid = MEM_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = CFG_DATA[i].attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

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

void readMemberConfigData(void) {
	uint8 i,j;
	readConfig(MEM_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(MEM_CFG_DATA)*MEM_ALM_SRC_N);
	if( CFG_DATA[0].attr[0] == 0xff ) {
		for (i = 0; i < MEM_ALM_SRC_N; ++i) {
			for (j = 0; j < MEM_TYPE_SIZE; ++j) {
				CFG_DATA[i].attr[j] = 0x0B;
			}
		}
		saveMemberCfg();
	}
}
bool saveMemberCfg(void) {
	saveConfig(MEM_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(MEM_CFG_DATA)*MEM_ALM_SRC_N);
	return true;
}

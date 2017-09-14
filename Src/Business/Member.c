/*
 * Member.c
 *
 *  Created on: 2017年9月14日
 *      Author: Administrator
 */
#include "../Business/Member.h"
#include "../inc/taskID.h"
#include "../Business/ConfigData.h"
#include "../inc/ErrCode.h"
#include "../Board/ChipSE0165B.h"

static xdata MEM_CFG CFG_DATA = {0};

void initMembConfig(void) {
	readMemberConfigData();
}

void readMemberConfigData(void) {
	uint8 i,j;
	readConfig(MEM_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(MEM_CFG));
	if( CFG_DATA.crc != 0x5A ) {
		for (i = 0; i < BD_VCGM_NUM; ++i) {
			for (j = 0; j < MEM_TYPE_SIZE; ++j) {
				CFG_DATA.member[i].attr[j] = 0x0B;
			}
		}
		saveMemberCfg();
	}
}
bool saveMemberCfg(void) {
	CFG_DATA.crc = 0x5A;
	saveConfig(MEM_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(MEM_CFG));
	return true;
}

MEM_CFG_DATA* getMembConfigStruct(uint8 sn) {
	if( sn < BD_VCGM_NUM ) {
		return &CFG_DATA.member[sn];
	}
	return 0;
}

uint8 getMembConfigData(uint8* buf);
bool setMembConfig(uint8* buf, uint8 len);

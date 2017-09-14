/*
 * E1Port.c
 *
 *  Created on: 2017年7月25日
 *      Author: Administrator
 */
#include "..\Business\E1Port.h"
#include "../Business/ConfigData.h"
#include "../inc/taskID.h"

static xdata E1_CFG CFG_DATA = {0};

void initE1PortConfig(void) {
	readE1ConfigData();

}

E1_CFG_DATA* getE1ConfigStruct(uint8 e1sn) {
	if(e1sn < BD_E1_NUM ) {
		return &CFG_DATA.e1[e1sn];
	}
	return 0;
}

void readE1ConfigData(void) {
	uint8 i,j;
	readConfig(E1_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(E1_CFG));
	if( CFG_DATA.crc != 0x5A ) {
		for (i = 0; i < BD_E1_NUM; ++i) {
			for (j = 0; j < E1_TYPE_SIZE; ++j) {
				CFG_DATA.e1[i].attr[j] = 0x0A;
			}
		}
		saveE1Cfg();
	}
}
bool saveE1Cfg(void) {
	CFG_DATA.crc = 0x5A;
	saveConfig(E1_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(E1_CFG));
	return true;
}

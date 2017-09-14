/*
 * EthPort.c
 *
 *  Created on: 2017年9月14日
 *      Author: Administrator
 */

#include "../Business/EthPort.h"
#include "../inc/ErrCode.h"
#include "../Business/ConfigData.h"
#include "../inc/taskID.h"
#include "../RTLLib/rtk_api_ext.h"

static xdata ETH_CFG CFG_DATA = {0};

void initEthPortConfig(void) {
	readEthConfigData();
}

ETH_CFG_DATA* getEthConfigStruct(uint8 sn) {
	if( sn < BD_ETH_NUM ) {
		return &CFG_DATA.ePort[sn];
	}
	return 0;
}


void readEthConfigData(void) {
	uint8 i,j;
	readConfig(ETH_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(CFG_DATA));
	if( CFG_DATA.crc != 0x5A ) {
		for (i = 0; i < BD_ETH_NUM; ++i) {
			for (j = 0; j < ETH_TYPE_SIZE; ++j) {
				CFG_DATA.ePort[i].attr[j] = 0x0A;
			}
		}
		saveEthCfg();
	}
}
bool saveEthCfg(void) {
	CFG_DATA.crc = 0x5A;
	saveConfig(ETH_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(CFG_DATA));
	return true;
}

uint8 getEthPConfigData(uint8* buf);
bool setEthPConfig(uint8* buf, uint8 len);

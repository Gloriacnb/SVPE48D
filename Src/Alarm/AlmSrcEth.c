/*
 * AlmSrcEth.c
 *
 *  Created on: 2017年9月6日
 *      Author: Administrator
 */

#include "../Alarm/AlmSrcEth.h"
#include "../inc/ErrCode.h"
#include "../Business/ConfigData.h"
#include "../inc/taskID.h"
#include "../RTLLib/rtk_api_ext.h"

static bool ifEthAlarm(uint8 sid, ALM_TYPE* types);
static bool saveEthCfg(void);
static xdata ETH_CFG_DATA CFG_DATA[ETH_ALM_SRC_N] = {0};

int initALMSrcETH(ALM_SRC* src, uint8 count) {
	uint8 i = 0;
	uint8 j = 0;
	if( src == 0 ) {
		return ERR_INPUT;
	}
	readEthConfigData();
	for (i = 0; i < count; ++i) {
		src[i].sid = ETH_ASID_BASE + i;
		src[i].saveCfg = saveEthCfg;
		src[i].typeCount = ETH_TYPE_SIZE;
		src[i].getAlarm = ifEthAlarm;
		for (j = 0; j < ETH_TYPE_SIZE; ++j) {
			src[i].type[j].tid = ETH_ATID_BASE+j;
			src[i].type[j].preState = false;
			src[i].type[j].actState = false;
			src[i].type[j].attr = CFG_DATA[i].attr[j];	//恢复配置
		}

	}
	return ERR_NONE;
}

bool ifEthAlarm(uint8 sid, ALM_TYPE* types) {
	rtk_port_linkStatus_t link;
	rtk_port_speed_t speed;
	rtk_port_duplex_t dup;
	if( types == 0 ) {
		return false;
	}
	rtk_port_phyStatus_get(SN(sid), &link, &speed, &dup);
	types[ETH_LNKDOWN].actState = (link == PORT_LINKDOWN)?true:false;
	types[ETH_SPEEDLOW].actState = (speed == PORT_SPEED_10M)?true:false;
	types[ETH_HALFDUP].actState = (dup == PORT_HALF_DUPLEX)?true:false;
	return true;
}

void readEthConfigData(void) {
	uint8 i,j;
	readConfig(ETH_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(ETH_CFG_DATA)*ETH_ALM_SRC_N);
	if( CFG_DATA[0].attr[0] == 0xff ) {
		for (i = 0; i < ETH_ALM_SRC_N; ++i) {
			for (j = 0; j < ETH_TYPE_SIZE; ++j) {
				CFG_DATA[i].attr[j] = 0x0A;
			}
		}
		saveEthCfg();
	}
}
bool saveEthCfg(void) {
	saveConfig(ETH_CFG_SECTOR, (uint8*)CFG_DATA, sizeof(ETH_CFG_DATA)*ETH_ALM_SRC_N);
	return true;
}

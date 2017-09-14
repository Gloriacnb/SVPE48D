/*
 * EthPort.h
 *
 *  Created on: 2017年9月14日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_ETHPORT_H_
#define SRC_BUSINESS_ETHPORT_H_

#include "../Board/BoardConfig.h"
#include "../Alarm/AlmSrcEth.h"

typedef struct ETH_ALM_ATTR_DATA_T{
	uint8 attr[ETH_TYPE_SIZE];
}ETH_CFG_DATA;

typedef struct ALL_ETH_CONFIG {
	ETH_CFG_DATA ePort[BD_ETH_NUM];
	uint8 crc;
}ETH_CFG;

void initEthPortConfig(void);

void readEthConfigData(void);
bool saveEthCfg(void);

ETH_CFG_DATA* getEthConfigStruct(uint8 sn);

uint8 getEthPConfigData(uint8* buf);
bool setEthPConfig(uint8* buf, uint8 len);

#endif /* SRC_BUSINESS_ETHPORT_H_ */

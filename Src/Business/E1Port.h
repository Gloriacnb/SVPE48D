/*
 * E1Port.h
 *
 *  Created on: 2017年7月25日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_E1PORT_H_
#define SRC_BUSINESS_E1PORT_H_

#include "../Alarm/AlmSrcE1.h"
#include "../Board/BoardConfig.h"

typedef struct E1_ALM_ATTR_DATA_T {
	uint8 attr[E1_TYPE_SIZE];
}E1_CFG_DATA;

typedef struct ALL_E1_CFG {
	E1_CFG_DATA e1[BD_E1_NUM];
	uint8 crc;
}E1_CFG;

void initE1PortConfig(void);

void readE1ConfigData(void);
bool saveE1Cfg(void);

E1_CFG_DATA* getE1ConfigStruct(uint8 e1sn);
#endif /* SRC_BUSINESS_E1PORT_H_ */

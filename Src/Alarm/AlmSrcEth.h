/*
 * AlmSrcEth.h
 *
 *  Created on: 2017年9月6日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCETH_H_
#define SRC_ALARM_ALMSRCETH_H_

#include "../inc/type.h"
#include "../Alarm/AlarmDefine.h"

enum {
	ETH_LNKDOWN,
	ETH_SPEEDLOW,
	ETH_HALFDUP,
	ETH_TYPE_SIZE,
};//定义E1的告警类型


typedef struct ETH_ALM_ATTR_DATA_T{
	uint8 attr[ETH_TYPE_SIZE];
}ETH_CFG_DATA;

int initALMSrcETH(ALM_SRC* src, uint8 cout);
void readEthConfigData(void);

#endif /* SRC_ALARM_ALMSRCETH_H_ */

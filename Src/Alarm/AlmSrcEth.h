/*
 * AlmSrcEth.h
 *
 *  Created on: 2017年9月6日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCETH_H_
#define SRC_ALARM_ALMSRCETH_H_

#include "../Alarm/AlarmDefine.h"

enum {
	ETH_LNKDOWN,
	ETH_SPEEDLOW,
	ETH_HALFDUP,
	ETH_TYPE_SIZE,
};//定义E1的告警类型

int initALMSrcETH(ALM_SRC* src, uint8 cout);

#endif /* SRC_ALARM_ALMSRCETH_H_ */

/*
 * AlmSrcE1.h
 *
 *  Created on: 2017年9月5日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCE1_H_
#define SRC_ALARM_ALMSRCE1_H_

#include "../inc/type.h"
#include "../Alarm/AlarmDefine.h"

enum {
	E1_LOS,
	E1_AIS,
	E1_LOF,
	E1_LOM,
	E1_CV,
	E1_CRC,
	E1_RDI,
	E1_TYPE_SIZE,
};//定义E1的告警类型

typedef struct E1_ALM_ATTR_DATA_T {
	uint8 attr[E1_TYPE_SIZE];
}E1_CFG_DATA;

int initALMSrcE1(ALM_SRC* src, uint8 cout);
void readE1ConfigData(void);

#endif /* SRC_ALARM_ALMSRCE1_H_ */

/*
 * AlmSrcMember.h
 *
 *  Created on: 2017年9月11日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCMEMBER_H_
#define SRC_ALARM_ALMSRCMEMBER_H_

#include "../inc/type.h"
#include "../Alarm/AlarmDefine.h"

enum {
	MEM_SQM,
	MEM_CRCERR,
	MEM_LOM2,
	MEM_LOM1,
	MEM_TYPE_SIZE,
};//定义E1的告警类型


typedef struct MEM_ALM_ATTR_DATA_T{
	uint8 attr[MEM_TYPE_SIZE];
}MEM_CFG_DATA;

int initALMSrcMEM(ALM_SRC* src, uint8 cout);
void readMemberConfigData(void);


#endif /* SRC_ALARM_ALMSRCMEMBER_H_ */

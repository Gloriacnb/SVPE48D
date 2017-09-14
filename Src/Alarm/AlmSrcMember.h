/*
 * AlmSrcMember.h
 *
 *  Created on: 2017年9月11日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCMEMBER_H_
#define SRC_ALARM_ALMSRCMEMBER_H_

#include "../Alarm/AlarmDefine.h"

enum {
	MEM_SQM,
	MEM_CRCERR,
	MEM_LOM2,
	MEM_LOM1,
	MEM_TYPE_SIZE,
};//定义E1的告警类型



int initALMSrcMEM(ALM_SRC* src, uint8 cout);


#endif /* SRC_ALARM_ALMSRCMEMBER_H_ */

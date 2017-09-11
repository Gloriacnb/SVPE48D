/*
 * AlmSrcVCG.h
 *
 *  Created on: 2017年9月8日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALMSRCVCG_H_
#define SRC_ALARM_ALMSRCVCG_H_

#include "../inc/type.h"
#include "../Alarm/AlarmDefine.h"

enum {
	VCG_ACKTMO,
	VCG_LOA,
	VCG_GIDM,
	VCG_DLYERR,
	VCG_MFIERR,
	GFP_SYNCLOS,
	GFP_CRCERR,
	GFP_FCSERR,
	VCG_TYPE_SIZE,
};//定义E1的告警类型


typedef struct VCG_ALM_ATTR_DATA_T{
	uint8 attr[VCG_TYPE_SIZE];
}VCG_CFG_DATA;

int initALMSrcVCG(ALM_SRC* src, uint8 cout);
void readVCGConfigData(void);


#endif /* SRC_ALARM_ALMSRCVCG_H_ */
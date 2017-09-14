/*
 * Member.h
 *
 *  Created on: 2017年9月14日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_MEMBER_H_
#define SRC_BUSINESS_MEMBER_H_

#include "../Board/BoardConfig.h"
#include "../Alarm/AlmSrcMember.h"

typedef struct MEM_ALM_ATTR_DATA_T{
	uint8 attr[MEM_TYPE_SIZE];
}MEM_CFG_DATA;

typedef struct ALL_MEM_CFG {
	MEM_CFG_DATA member[BD_VCGM_NUM];
	uint8 crc;
}MEM_CFG;

void initMembConfig(void);

void readMemberConfigData(void);
bool saveMemberCfg(void);

MEM_CFG_DATA* getMembConfigStruct(uint8 sn);

uint8 getMembConfigData(uint8* buf);
bool setMembConfig(uint8* buf, uint8 len);

#endif /* SRC_BUSINESS_MEMBER_H_ */

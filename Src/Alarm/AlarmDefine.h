/*
 * AlarmDefine.h
 *
 *  Created on: 2017年9月5日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALARMDEFINE_H_
#define SRC_ALARM_ALARMDEFINE_H_

#include "../inc/type.h"

#define MAX_TYPES	8

typedef struct ALM_TYPE_T {
	uint8 tid;				//告警类型ID，由各个告警源统一编码
	bool preState;			//上一次采集到的状态 true表示有告警
	bool actState;			//本次采集到的状态
	uint8 attr;				//告警属性 bit0~1 level, bit2 mask, bit3 report
}ALM_TYPE;

typedef struct ALM_SRC_T{
	uint8 sid;					//告警源ID
	ALM_TYPE type[MAX_TYPES];	//告警类型，一维数组，每个元素对应一个类型,最多8个
	uint8 typeCount;			//告警类型数量
	bool (*getAlarm)(uint8 sid, ALM_TYPE* types);	//告警源获取告警函数指针
	bool (*saveCfg)(void);		//属性存储函数指针
}ALM_SRC;

#define GET_LEVEL(attr) 	( (attr)&0x3 )
#define IF_MASK(attr)		( ( (attr)>>2 )%2 != 0 )
#define IF_REPORT(attr)		( ( (attr)>>3 )%2 != 0 )

#define E1_ASID_BASE	0x10
#define VCG_ASID_BASE	0x20
#define MEM_ASID_BASE	0x30
#define ETH_ASID_BASE	0x40

#define E1_ATID_BASE		(E1_ASID_BASE 	+ 0x80)
#define VCG_ATID_BASE		(VCG_ASID_BASE 	+ 0x80)
#define MEM_ATID_BASE		(MEM_ASID_BASE + 0x80)
#define ETH_ATID_BASE		(ETH_ASID_BASE 	+ 0x80)

#define SN(id)			((id)&0xf)

typedef enum {
	ASID_E1_1 = E1_ASID_BASE, ASID_E1_2, ASID_E1_3, ASID_E1_4,
	ASID_VCG = VCG_ASID_BASE,
	ASID_MEMB = MEM_ASID_BASE,
	ASID_ETH_1 = ETH_ASID_BASE, ASID_ETH_2, ASID_ETH_3, ASID_ETH_4,
};//定义告警源的ID

#define E1_ALM_SRC_N	4	//4路E1告警
#define VCG_ALM_SRC_N	1	//1路VCG告警
#define MEM_ALM_SRC_N	4	//4路VCG member告警
#define ETH_ALM_SRC_N	4	//4路以太接口告警

/*不同告警源在总告警源数组中的元素序号定义*/
#define E1_SN_BASE		0								//E1告警源的起始序列号
#define VCG_SN_BASE 	(E1_SN_BASE + E1_ALM_SRC_N)		//VCG告警源的起始序列号
#define MEM_SN_BASE		(VCG_SN_BASE + VCG_ALM_SRC_N)	//VCG Member起始序列号
#define ETH_SN_BASE		(MEM_SN_BASE + MEM_ALM_SRC_N)	//以太告警源的起始序列号

/*告警源的总数量*/
#define ALM_SRC_N (	E1_ALM_SRC_N + \
					VCG_ALM_SRC_N + \
					MEM_ALM_SRC_N + \
					ETH_ALM_SRC_N)


#endif /* SRC_ALARM_ALARMDEFINE_H_ */

/*
 * AlarmModule.c
 *
 *  Created on: 2017年9月4日
 *      Author: Administrator
 */

#include "../Alarm/AlarmModule.h"
#include "../inc/taskID.h"
#include "../Alarm/AlmSrcE1.h"
#include "../Alarm/AlmSrcEth.h"
#include "../Alarm/AlmSrcVCG.h"
#include "../Alarm/AlmSrcMember.h"
#include "../Alarm/AlarmDefine.h"
#include <RTX51TNY.H>
#include <stdio.h>
#include "../communication/dcc.h"
#include "../inc/ErrCode.h"
#include "../STCLib/USART.h"
#include "../communication/console.h"
#include "../communication/dcc.h"


xdata ALM_SRC AS[ALM_SRC_N] = {0};	//告警源集合
xdata uint8 ACT_SRC = 0;	//初始化激活的告警源总数
void runAlarmModule(void) {
	if( initALMSrcE1(&AS[E1_SN_BASE], E1_ALM_SRC_N) == ERR_NONE ) {
		ACT_SRC += E1_ALM_SRC_N;
	}
	if( initALMSrcETH(&AS[ETH_SN_BASE], ETH_ALM_SRC_N) == ERR_NONE ) {
		ACT_SRC += ETH_ALM_SRC_N;
	}
	if( initALMSrcVCG(&AS[VCG_SN_BASE], VCG_ALM_SRC_N) == ERR_NONE ) {
		ACT_SRC += VCG_ALM_SRC_N;
	}
	if( initALMSrcMEM(&AS[MEM_SN_BASE], MEM_ALM_SRC_N) == ERR_NONE ) {
		ACT_SRC += MEM_ALM_SRC_N;
	}
	os_create_task(tsk_alarm_proc);
}

/*
 * 告警处理任务，
 * 遍历所有告警源和告警类型
 * 采集告警状态
 * 与上一次值比对，有变化且使能上报则上报告警
 */
void processAlarm (void) _task_ tsk_alarm_proc  {
	uint8 i = 0;
	uint8 j = 0;
	bool newSt = false;	//true 表示有告警， false表示无告警
	putchar(0xc0);
	while(1) {
		os_wait (K_IVL, 200, 0);
		for (i = 0; i < ACT_SRC; ++i) {	//遍历所有告警源
			AS[i].getAlarm(AS[i].sid, AS[i].type);		//告警源读取其所有告警状态
			for (j = 0; j < AS[i].typeCount; ++j) {	//遍历所有告警类型
				if( IF_MASK(AS[i].type[j].attr) ) {
					/*被屏蔽则无告警*/
					newSt = false;
				}
				else {
					newSt = AS[i].type[j].actState;
				}
				if( newSt != AS[i].type[j].preState ) {
					/*有变化*/
					if( IF_REPORT(AS[i].type[j].attr) ) {
						/*使能上报*/
						reportAlarm(AS[i].sid, AS[i].type[j].tid, newSt, GET_LEVEL(AS[i].type[j].attr));
					}
					AS[i].type[j].preState = newSt;
				}
			}//告警类型遍历结束
		}//告警源遍历结束
	}
}


void reportAlarm(uint8 sid, uint8 tid, bool raise, uint8 level) {
//	printf("0x%bx, 0x%bx, %bd\r\n", srcsn, typesn, raise);
	CMD_FRAME xdata f = {0};
	f.ttype = LOCAL_CMD;
	f.tlen = 6;
	f.tdata[0] = 0xAA;
	f.tdata[1] = sid;
	f.tdata[2] = tid;
	f.tdata[3] = raise?1:0;
	f.tdata[4] = level;
	f.tdata[5] = 0x5A;
	consoleSendFrame(&f);
	f.ttype = REMOTE_CMD;
	dccSendFrame(&f);
}

/*告警属性配置接口*/
uint8 getAlarmAttribute(uint8 src, uint8 type) {
	return AS[SN(src)].type[SN(type)].attr;
}
bool setAlarmAttribute(uint8 src, uint8 type, uint8 newattr) {
	AS[SN(src)].type[SN(type)].attr = newattr;
	return AS[SN(src)].saveCfg();
}


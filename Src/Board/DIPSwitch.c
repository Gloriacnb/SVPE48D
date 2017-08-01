/*
 * DIPSwitch.c
 *
 *  Created on: 2017年7月25日
 *      Author: Administrator
 */
#include "..\Board\DIPSwitch.h"
#include "../Board/ChipSE0165B.h"
#include "..\STCLib\config.h"
#include "../RTLLib/rtk_api_ext.h"
#include "../inc/taskID.h"
#include <RTX51TNY.H>

#define DPSW1	P10
#define DPSW2	P11
#define DPSW3	P12
#define DPSW4	P13

#define ON	1
#define OFF 0
/*
 * 初始化拨码开关相关pin脚为输入
 */
void initDIPSwitch(void) {
	uint8 i = 0;
	rtk_portmask_t isolate;

	if( DPSW1 == ON ) {
		if( DPSW2 == ON ) {
			//超长包
			rtk_switch_maxPktLen_set(MAXPKTLEN_2000B);
		}
		else {
			rtk_switch_maxPktLen_set(MAXPKTLEN_1536B);

		}
		if( DPSW3 == ON ) {
			//以太隔离
			isolate.bits[0] = 0;
			isolate.bits[0] |= (1 << 4);
			for (i = 0; i < 4; ++i) {
				rtk_port_isolation_set(i, isolate);
			}
			isolate.bits[0] = 0x1f;
			rtk_port_isolation_set(4, isolate);
		}
		if( DPSW4 == ON ) {
			//恢复时钟
			setE1ClockMode(1);
			os_create_task(tsk_e1_rclock);
		}
		else {
			//本地时钟
			setE1ClockMode(0);
		}
	}
}

/*
 * E1恢复时钟时，自动选择可用E1任务
 * 每秒轮询一次所有E1，将第一个无告警E1做为恢复时钟源。
 */
void selectE1Rclock(void) _task_ tsk_e1_rclock {
	xdata uint8 currntE1 = 0xff;
	xdata int e1Alarm = 0;
	while(1) {
		xdata uint8 i = 0;
		os_wait(K_TMO, 100, 0);
		for (i = 0; i < 4; ++i) {
			e1Alarm = getE1Alarm(i);
			if( e1Alarm == 0 ) {
				//第一个无告警的E1
				if( i == currntE1 ) {
					break;	//跟原恢复时钟一致，避免重复写寄存器，跳出for循环
				}
				else {
					currntE1 = i;
					setE1RecoverClockSouce(currntE1);
				}
				break; //已经找到可用E1，跳出for循环
			}
		}
	}
}

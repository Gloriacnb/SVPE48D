/*
 * LED.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */
#include "..\Board\LED.h"
#include "..\STCLib\config.h"
#include <RTX51TNY.H>
#include "..\inc\taskID.h"
#include "..\board\ChipSE0165B.h"
#include "..\Business\E1Port.h"
#include <stdio.h>
#define LED_RUN P36
#define LED_TOP_ALM P35

/*
 * bdata 和相关的 sbit变量必须设置为全局变量
 */
uint8 bdata LED_STA = 0;		//点灯输出字节
	sbit E1_1_tsf = LED_STA^0;
	sbit E1_1_los = LED_STA^1;
	sbit E1_2_tsf = LED_STA^2;
	sbit E1_2_los = LED_STA^3;
	sbit E1_3_tsf = LED_STA^4;
	sbit E1_3_los = LED_STA^5;
	sbit E1_4_tsf = LED_STA^6;
	sbit E1_4_los = LED_STA^7;

/*
 * 建立点灯任务  tsk_led 和 tsk_led_e1
 */
void initLEDs(void) {

    os_create_task(tsk_run);				//建立Run 灯任务
    os_create_task(tsk_led_e1);
}

/*
 * 点亮所有E1 LED灯
 */
void turnOnE1LEDs(void) {
	sendByteSDO0(0xff);
}

/*
 * 熄灭所有E1 LED灯
 */
void turnOffE1LEDs(void) {
	sendByteSDO0(0);
}

/*
 * 刷新E1 指示灯
 * 	byte
 	bit E1_1_tsf = ledSta^0;
	bit E1_1_los = ledSta^1;
	bit E1_2_tsf = ledSta^2;
	bit E1_2_los = ledSta^3;
	bit E1_3_tsf = ledSta^4;
	bit E1_3_los = ledSta^5;
	bit E1_4_tsf = ledSta^6;
	bit E1_4_los = ledSta^7;
 */
void refreshE1LEDs(uint8 ledSta) {
	sendByteSDO0(ledSta);
}

/*
 * run灯任务，每0.5s反转 run灯
 */
void ledRun(void) _task_ tsk_run {
	while(1) {
		LED_RUN ^= 1;
		os_wait (K_TMO, 50, 0);
	}
}


/*
 * E1 点灯任务
 * 	注意：此任务为临时版本，当后期实现告警模块时，E1 LED做为告警输出的部分，到时需要重构
 */
void ledE1(void) _task_ tsk_led_e1 {
	int xdata E1_ALARM[4] = {0};
	uint8 xdata i = 0;

	/*E1接口指示灯全亮两秒后熄灭*/
	turnOnE1LEDs();
	os_wait (K_TMO, 200, 0);
	turnOffE1LEDs();

	while(1) {
		for (i = 0; i < 4; ++i) {
			E1_ALARM[i] = getE1Alarm(i);
		}
        E1_1_tsf = ((E1_ALARM[0]&0xaf)!=0);
        E1_2_tsf = ((E1_ALARM[1]&0xaf)!=0);
        E1_3_tsf = ((E1_ALARM[2]&0xaf)!=0);
        E1_4_tsf = ((E1_ALARM[3]&0xaf)!=0);
        
        if( ((E1_ALARM[0]>>4)&1) && (~(E1_ALARM[0]>>0)&1) )
        	E1_1_los ^= 1;						//CV告警work灯闪烁
        else
        	E1_1_los = ~(E1_ALARM[0]>>0)&1;

        if( ((E1_ALARM[1]>>4)&1)  && (~(E1_ALARM[1]>>0)&1) )
        	E1_2_los ^= 1;
        else
        	E1_2_los = ~(E1_ALARM[1]>>0)&1;

        if( ((E1_ALARM[2]>>4)&1)  && (~(E1_ALARM[2]>>0)&1) )
        	E1_3_los ^= 1;
        else
        	E1_3_los = ~(E1_ALARM[2]>>0)&1;

        if( ((E1_ALARM[3]>>4)&1)  && (~(E1_ALARM[3]>>0)&1) )
        	E1_4_los ^= 1;
        else
        	E1_4_los = ~(E1_ALARM[3]>>0)&1;

        refreshE1LEDs(LED_STA);
		os_wait (K_TMO, 100, 0);
	}
}

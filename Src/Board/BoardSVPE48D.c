/*
 * BoardSVPE48D.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */
#include "..\Board\BoardSVPE48D.h"
#include "..\communication\console.h"
#include "..\communication\dcc.h"
#include "..\Board\LED.h"
#include "..\Board\ChipSE0165B.h"
#include "..\Board\ChipRTL8306E.h"
#include "../Board/DIPSwitch.h"
#include "..\STCLib\GPIO.h"
#include "..\inc\taskID.h"
#include <stdio.h>
#include <RTX51TNY.H>
#include "../STCLib/delay.h"

void boardInit(void) {

	/*设置P0~P5均为准双向模式*/
	uint8 i = 0;
    GPIO_InitTypeDef info = {0};
    info.Pin = GPIO_Pin_All;
    info.Mode = GPIO_PullUp;
    for (i = 0; i < 6; ++i) {
		GPIO_Inilize(i, &info);
    }
    
    /*配置UART1为 8位数据位，一位停止位，波特率9600*/
	initConsole();
	printf("Wait Board power up!\r\n");
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	delay_ms(20 * 12);
	printf("Start initial board!\r\n");

	if( !initSE0165B() ) {
        printf("Init SE0165B failed!!\r\n");
    }
    initDCC();
	initLEDs();
	if( !initRTL8306E() ) {
		printf("Init RTL8306E failed!!\r\n");
	}
	initDIPSwitch();
	printf("Board initial completed.\r\n");

//	boardAutoTest();
}
void boardAutoTest(void) {
	os_create_task(tsk_test);
	printf("Board test completed.\r\n");
}


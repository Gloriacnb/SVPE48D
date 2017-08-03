/*
 * dcc.c
 *
 *  Created on: 2017年7月29日
 *      Author: Administrator
 */
#include "..\communication\dcc.h"
#include "..\Board\ChipSE0165B.h"
#include "../inc/taskID.h"
#include "../communication/console.h"
#include "..\STCLib\USART.h"
#include "../RTLLib/rtk_api_ext.h"
#include "../Business/Device.h"
#include "../STCLib/delay.h"
#include <RTX51TNY.H>
#include <string.h>

static xdata CMD_FRAME dccframe;
xdata uint8 looped = 0;

void initDCC(void) {
	os_create_task(tsk_dcc_rcv);		//dcc收包任务
	os_create_task(tsk_loop_detect);	//环回维护任务
	os_create_task(tsk_send_loop);		//建立环回帧发送任务
}

void dccSendFrame(CMD_FRAME* f) {
	uint8 i;
	writeTCMFData(0, FRAME_HEAD);
	writeTCMFData(1, f->ttype);
	writeTCMFData(2, f->tlen);
	for (i = 0; i < f->tlen; ++i) {
		writeTCMFData(3+i, f->tdata[i]);
	}
	setTCMFLength(f->tlen + 3);			//指定发送帧总长度
	startTCMF();
	delay_ms(1);
//	while(!ifTCMFOver());
}

/*
 * dcc收包任务
 * 轮询方式读取CMF帧
 * 私有协议解包
 * 环回探测包处理
 */
void dccRcvFrame(void) _task_ tsk_dcc_rcv {
	xdata uint8 rcvLen = 0;
	xdata uint8 i;
	while(1) {
		os_wait(K_TMO, 5, 0);
		if( ifRCMFReady() ) {

			rcvLen = getRCMFLength();
			if( rcvLen > MAX_FRAME_LEN || rcvLen < MIN_FRAME_LEN ) {
				continue;
			}
			if( readRCMFData(0) != FRAME_HEAD ) {
				continue;
			}
			dccframe.rtype = readRCMFData(1);
			dccframe.rlen = readRCMFData(2);
			for (i = 0; i < dccframe.rlen; ++i) {
				dccframe.rdata[i] = readRCMFData(i+3);
			}
			if( dccframe.rtype == LOCAL_CMD ) {
				processCMD(&dccframe);
				dccframe.ttype = REMOTE_CMD;
				dccSendFrame(&dccframe);
			}
			else if( dccframe.rtype == REMOTE_CMD ){
				dccframe.ttype = dccframe.rtype;
				dccframe.tlen = dccframe.rlen;
				memcpy(dccframe.tdata, dccframe.rdata, dccframe.tlen);
				consoleSendFrame(&dccframe);
			}
			else if( dccframe.rtype == LOOP_DETEC ) {
				if( (dccframe.rdata[0] | (dccframe.rdata[1]<<8)) == getSerialNumber() ) {
					//检测到环回
					os_send_signal(tsk_loop_detect);
				}

			}
		}
	}
}

/*
 * 间隔0.5s发送环回探测包
 */
void sendLoopFrame(void) _task_ tsk_send_loop {
	xdata CMD_FRAME loopframe;
	xdata uint16 sn = getSerialNumber();
	loopframe.ttype = LOOP_DETEC;
	loopframe.tlen = 3;
	loopframe.tdata[0] = sn & 0xff;
	loopframe.tdata[1] = (sn>>8) & 0xff;
	loopframe.tdata[2] = CMD_OK;
	while(1) {
		os_wait(K_TMO, 70, 0);
		dccSendFrame(&loopframe);
	}
}

/*
 * 检测到环回后的处理任务
 * 	一旦检测到环回，则关闭所有LAN口
 * 	如果连续两秒都没有检测到环回，则打开LAN口
 */
void loopDetection(void) _task_ tsk_loop_detect {
	while(1) {
		xdata uint8 i;
		xdata char event;
		event = os_wait(K_SIG | K_TMO, 200, 0);
		switch(event) {
		case SIG_EVENT:	//检测到环回
			if( looped == 0 ) {
				looped = 1;
				for (i = 0; i < 4; ++i) {
					rtl8306e_regbit_set(i, 0, 11, 0, 1);	//power down
				}
			}
			break;
		case TMO_EVENT:	//连续两秒没有检测到环回，则认为环回已经取消
			looped = 0;
			for (i = 0; i < 4; ++i) {
				rtl8306e_regbit_set(i, 0, 11, 0, 0);	//power down
			}
			break;
		default:
			break;
		}
	}
}

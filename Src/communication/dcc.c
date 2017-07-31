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
#include <RTX51TNY.H>
#include <string.h>

static xdata CMD_FRAME dccframe;

void initDCC(void) {
	os_create_task(tsk_dcc_rcv);
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
	while(!ifTCMFOver());
}

void dccRcvFrame(void) _task_ tsk_dcc_rcv {
	uint8 rcvLen = 0;
	uint8 i;
	while(1) {
		os_wait(K_TMO, 10, 0);
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
			else {
				dccframe.ttype = dccframe.rtype;
				dccframe.tlen = dccframe.rlen;
				memcpy(dccframe.tdata, dccframe.rdata, dccframe.tlen);
				consoleSendFrame(&dccframe);
			}
		}
	}
}

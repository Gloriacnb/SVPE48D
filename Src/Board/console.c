/*
 * console.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */
#include "..\Board\console.h"
#include "..\STCLib\USART.h"
#include "..\inc\taskID.h"
#include <RTX51TNY.H>
#include <stdio.h>

/*收包状态机*/
enum {
	STATE_IDLE = 0,		//起始状态,只接收0x7E
	STATE_LENGTH,		//收到帧头0x7E进入此状态，表示收帧开始
	STATE_TYPE,			//接收到长度字节后进入
	STATE_DATA,			//接收到类型字节后进入
	STATE_TRANSLATE,	//在非IDLE状态接收到0x7E，进入此状态，表示转译
	STATE_OVER,

}CONSOLE_FSM;

enum {
	LOCAL_CMD = 0,
	REMOTE_CMD = 1,
}CMD_TYPE;

static idata CMD_FRAME frame;

#define FRAME_HEAD	0x7E
#define TRANSLATE_HEAD	0x7D
#define TRANSLATE_TAIL1	0x5E
#define TRANSLATE_TAIL2	0x5D

/*
 * 将管理串口初始化为
 * 	8位数据位，1位停止位，无校验，波特率9600，使用T2做为波特率发生器
 * 	串口中断使能，中断高优先级，接收使能
 */
void initConsole(void) {
	/*初始化COM1*/
	COMx_InitDefine info = {0};
	info.BaudRateDouble = DISABLE;
	info.Morecommunicate = DISABLE;
	info.UART_BRT_Use = BRT_Timer2;
	info.UART_P_SW = UART1_SW_P30_P31;
	info.UART_BaudRate = 9600;
	info.UART_Interrupt = ENABLE;
	info.UART_Mode = UART_8bit_BRTx;
	info.UART_Polity = PolityHigh;
	info.UART_RXD_TXD_Short = DISABLE;
	info.UART_RxEnable = ENABLE;
	USART_Configuration(USART1, &info);

	/*建立串口收包任务*/
	os_create_task(tsk_console_rcv);
}

void serialRcvFrame(void) _task_ tsk_console_rcv {
	char c = 0;
	uint8 i = 0;
	uint8 fsm = STATE_IDLE;
	uint8 rcvedBytes = 0;
	while(1) {
		c = _getkey();
		if( (c == FRAME_HEAD) && (fsm != STATE_IDLE) ) {
			//非IDLE状态下收到帧头，则认为是新一帧的开始，之前的帧被丢弃
			fsm = STATE_TYPE;
			rcvedBytes = 0;
			continue;
		}
		switch( fsm ) {
		case STATE_IDLE:
			if( c == FRAME_HEAD ) {
				fsm = STATE_TYPE;
				rcvedBytes = 0;
			}
			break;
		case STATE_TYPE:
			if( (c != REMOTE_CMD) && (c!= LOCAL_CMD) ) {
				/*类型非法，复位状态机*/
				fsm = STATE_IDLE;
				rcvedBytes = 0;
			}
			else {
				frame.rtype = c;
				fsm = STATE_LENGTH;
			}
			break;
		case STATE_LENGTH:
			if( (c > MAX_FRAME_LEN) || (c < MIN_FRAME_LEN)) {
				/*长度非法，复位状态机*/
				fsm = STATE_IDLE;
				rcvedBytes = 0;
			}
			else {
				frame.rlen = c;
				fsm = STATE_DATA;
			}
			break;
		case STATE_DATA:
			if( c == TRANSLATE_HEAD ) {
				fsm = STATE_TRANSLATE;
			}
			else {
				frame.rdata[rcvedBytes++] = c;
				if( rcvedBytes == frame.rlen ) {
					fsm = STATE_OVER;
				}
			}
			break;
		case STATE_TRANSLATE:
			if( c!= TRANSLATE_TAIL1 && c!= TRANSLATE_TAIL2 ) {
				/*非法转译字符，复位状态机*/
				fsm = STATE_IDLE;
				rcvedBytes = 0;
			}
			else {
				frame.rdata[rcvedBytes++] = c+0x20;
				if( rcvedBytes == frame.rlen ) {
					fsm = STATE_OVER;
				}
				else {
					fsm = STATE_DATA;
				}
			}
			break;
		case STATE_OVER: //never reached
		default:
			fsm = STATE_IDLE;
			frame.rlen = 0;
			break;
		}
		if( fsm == STATE_OVER ) {
			putchar(0x7E);
			putchar(frame.rtype);
			putchar(frame.rlen);
			for (i = 0; i < frame.rlen; ++i) {
				putchar(frame.rdata[i]);
			}
			fsm = STATE_IDLE;
		}
	}
}

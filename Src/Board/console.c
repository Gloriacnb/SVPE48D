/*
 * console.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */
#include "..\Board\console.h"
#include "..\STCLib\USART.h"
/*
 * 将管理串口初始化为
 * 	8位数据位，1位停止位，无校验，波特率9600，使用T2做为波特率发生器
 * 	串口中断使能，中断高优先级，接收使能
 */
void initConsole(void) {
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

}

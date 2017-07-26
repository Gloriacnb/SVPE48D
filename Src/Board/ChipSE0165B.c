/*
 * ChipSE0165B.c
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 *      SE0165B是一款EOP芯片，完成E1/ETH的协议转换，是SVPE48D的核心芯片
 *      芯片与单片机通过并口相连，对其寄存器访问映射为扩展内存的访问
 */
#include "..\Board\ChipSE0165B.h"
#include "..\STCLib\config.h"
#include "..\STCLib\delay.h"
#include "..\STCLib\GPIO.h"
#include <absacc.h>
#include <stdio.h>

#include <RTX51TNY.H>
#include "..\inc\taskID.h"
#define SE0165B_CS 		P37		//片选信号，低（0）有效
/*
 * 读取SE0165B寄存器
 * 		regaddr 寄存器地址
 * 		return 寄存器值
 */
uint8 readSE0165B(uint16 regaddr) {
	uint8 regdata = 0;
	ES = 0;
	ExternalRAM_enable();	//切换到片外RAM
	SE0165B_CS = 0;			//片选生效
	regdata = XBYTE[regaddr];
	SE0165B_CS = 1;
	InternalRAM_enable();
	ES = 1;
	return regdata;
}

/*
 * 写入SE0165B寄存器
 * 		regaddr 寄存器地址
 * 		regdata 待写入数据
 *
 * 注意：SE0165B有写保护寄存器，本函数自动解除写保护，写完成后打开写保护
 */
void writeSE0165B(uint16 regaddr, uint8 regdata) {
	ES = 0;
	ExternalRAM_enable();	//切换到片外RAM
	SE0165B_CS = 0;			//片选生效
	XBYTE[SE0165B_GLOBAL_WLOCK_REG] = 0xAA;		//关闭写保护
	XBYTE[regaddr] = regdata;
	XBYTE[SE0165B_GLOBAL_WLOCK_REG] = 0x00;		//打开写保护
	SE0165B_CS = 1;
	InternalRAM_enable();
	ES = 1;
}

/*
 * 芯片初始化函数
 *  芯片软复位
 * 	读取芯片ID，判断芯片是否在位
 */
bool initSE0165B(void) {
    uint16 chipID = 0x0165;
	softReset();
	if( readChipID() != SE0165B_CHIPID ) {
		return false;
	}
    setDefault();
	return true;
}


/*
 * 软复位芯片
 */
void softReset(void) {
	writeSE0165B(SE0165B_GLOBAL_RSTLOOP1_REG, 0x01);
	delay_ms(5);
	writeSE0165B(SE0165B_GLOBAL_RSTLOOP1_REG, 0x00);
    delay_ms(5);
}

/*
 * 读芯片ID
 */
uint16 readChipID(void) {
	return (readSE0165B(SE0165B_GLOBAL_CHIP_IDH) << 8) | readSE0165B(SE0165B_GLOBAL_CHIP_IDL);
}

/*
 * 将芯片配置为默认状态
 * LCAS VCAT均使能
 *	E1 1~4支路使能
 */
void setDefault(void) {
	char i = 0;
	writeSE0165B(SE0165B_LCAS_TVCG_CFG, 0x03);	//发送方向 LCAS VCAT使能
	writeSE0165B(SE0165B_LCAS_RVCG_CFG, 0x03);	//接收方向 LCAS VCAT使能

	for (i = 0; i < 4; ++i) {
		//支路1-4使能
		writeSE0165B(SE0165B_LCAS_TTRIB_USE_SQ(i), 0x1f);
		writeSE0165B(SE0165B_LCAS_RTRIB_USE_SQ(i), 0x1f);
	}
}

/*
 * 串行输出Byte0到SDO0
 *
 */
void sendByteSDO0(uint8 byte) {
	writeSE0165B(SE0165B_GLOBAL_SDO0BYTE0_REG, byte);			//写入待发送字节
	while( (readSE0165B(SE0165B_GLOBAL_SDIO_OPT) & 1) != 0 ); 	//判断是否忙
	writeSE0165B(SE0165B_GLOBAL_SDIO_OPT, (1<<7) | (1<<6) );	//bit7 req; bit6 send
}

/*
 * 串行输入Byte0 from SDO0
 */
uint8 getByteSDO0(void) {
	while( (readSE0165B(SE0165B_GLOBAL_SDIO_OPT) & 1) != 0 ); 	//判断是否忙
	writeSE0165B(SE0165B_GLOBAL_SDIO_OPT, (1<<7) );	//bit7 req; bit6 send
	return readSE0165B(SE0165B_GLOBAL_SDIBYTE0_REG);

}

/*
 * 获取E1接口告警
 * portN E1序号 0~7
 * return 	bit9 EBIT2
 * 			bit8 EBIT1
 * 			bit7 RDI
 * 			bit6 TSF	总告警
 * 			bit5 CRC
 * 			bit4 CV
 * 			bit3 LOM
 * 			bit2 LOF
 * 			bit1 AIS
 * 			bit0 LOS
 */
int getE1Alarm(uint8 portN) {
	if(portN >= SE0165B_E1_MAXNUM ) {
		return -1;
	}
	return (readSE0165B(SE0165B_E1_ALARM2(portN))<<7) | readSE0165B(SE0165B_E1_ALARM1(portN));
}

void testChipSE0165B(void) _task_ tsk_test {
	uint16 chipID = 0x0165;
	while(1) {
		os_wait(K_TMO, 100, 0);

	    chipID = readChipID();
	    printf("chipID: 0x%04x\r\n", chipID);
	}
}

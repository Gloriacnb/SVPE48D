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
//static bool FIRST_MDIO = true;

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
 * 写入SE0165B寄存器 并回读校验
 * 		regaddr 寄存器地址
 * 		regdata 待写入数据
 * 		bitmask 需要校验的bit位，只校验为1的
 *
 * 注意：SE0165B有写保护寄存器，本函数自动解除写保护，写完成后打开写保护
 */

bool writeSE0165BVerify(uint16 regaddr, uint8 regdata, uint8 bitmask) {
	uint8 regv = 0;
	writeSE0165B(regaddr, regdata);
	regv = readSE0165B(regaddr);
	return (regdata & bitmask) == (regv & bitmask);
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
    return setDefault();
}


/*
 * 软复位芯片
 */
void softReset(void) {
	writeSE0165B(SE0165B_GLOBAL_RSTLOOP1_REG, 0x01);
    delay_ms(5*12);
	writeSE0165B(SE0165B_GLOBAL_RSTLOOP1_REG, 0x00);
    delay_ms(5*12);
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
bool setDefault(void) {
	char i = 0;
	if( !writeSE0165BVerify(SE0165B_LCAS_TVCG_CFG, 0x03, 0xff) ) {	//发送方向 LCAS VCAT使能
		return false;
	}
	if( !writeSE0165BVerify(SE0165B_LCAS_RVCG_CFG, 0x03, 0xff) ) {	//接收方向 LCAS VCAT使能
		return false;
	}

	for (i = 0; i < 4; ++i) {
		//支路1-4使能
		if( !writeSE0165BVerify(SE0165B_LCAS_TTRIB_USE_SQ(i), 0x1f, 0x1f) ) {
			return false;
		}
		if( !writeSE0165BVerify(SE0165B_LCAS_RTRIB_USE_SQ(i), 0x1f, 0x1f) ) {
			return false;
		}
	}
	return true;
}

/*
 * 串行输出Byte0到SDO0 用于E1 LED
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

/*
 * 读smi
 * 	phy 8306E phy地址
 * 	phyreg 8306E 寄存器地址
 */
uint16 readMDIO(uint8 phy, uint8 phyreg) {
	writeSE0165B(SE0165B_GLOBAL_PHYADDR_REG, phy);
	writeSE0165B(SE0165B_GLOBAL_PHYREGADDR_REG, phyreg);
	writeSE0165B(SE0165B_GLOBAL_MDIOCONTROL_REG, MDIO_START);
//	if( !FIRST_MDIO ) {
//		while( (readSE0165B(SE0165B_GLOBAL_MDIOCONTROL_REG) & MDIO_READY) == 0 );
//	}
//	else {
//		FIRST_MDIO = false;
//		delay_ms(5);
//	}
	delay_ms(1);
	return (readSE0165B(SE0165B_GLOBAL_MDIORDATH_REG)<<8) | readSE0165B(SE0165B_GLOBAL_MDIORDATL_REG);
}

/*
 * 写smi
 *
 */
bool writeMDIO(uint8 phy, uint8 phyreg, uint16 regData) {
	if(phy > 0x1f) {
		return false;
	}
	writeSE0165B(SE0165B_GLOBAL_PHYADDR_REG, phy);
	writeSE0165B(SE0165B_GLOBAL_PHYREGADDR_REG, phyreg);
	writeSE0165B(SE0165B_GLOBAL_MDIOWDATH_REG, (regData>>8) & 0xff);
	writeSE0165B(SE0165B_GLOBAL_MDIOWDATL_REG, regData & 0xff);
//	if( !FIRST_MDIO ) {
//		while( (readSE0165B(SE0165B_GLOBAL_MDIOCONTROL_REG) & MDIO_READY) == 0 );
//	}
//	else {
//		FIRST_MDIO = false;
//		delay_ms(1);
//	}
	writeSE0165B(SE0165B_GLOBAL_MDIOCONTROL_REG, MDIO_START | MDIO_WRITE);
	delay_ms(1);
	return true;
}

/*for cmf 读写*/
void writeTCMFData(uint8 sn, uint8 b) {
	writeSE0165B(SE0165B_GFP_TCMF_DAT_INDEX, sn);
	writeSE0165B(SE0165B_GFP_TCMF_DAT, b);
}

void setTCMFLength(uint8 len) {
	writeSE0165B(SE0165B_GFP_TCMF_DAT_LEN, len);
}
bool ifTCMFOver(void) {
	return (readSE0165B(SE0165B_GFP_TCMF_CTRL) & (1<<6)) != 0;
}
void startTCMF(void) {
	writeSE0165B(SE0165B_GFP_TCMF_CTRL, (1<<7));
}

bool ifRCMFReady(void) {
	uint8 v = readSE0165B(SE0165B_GFP_RCMF_READY);
	return (v & 1) == 1;
}
uint8 getRCMFLength(void) {
	return readSE0165B(SE0165B_GFP_RCMF_DAT_LEN);
}
uint8 readRCMFData(uint8 sn) {
	writeSE0165B(SE0165B_GFP_RCMF_DAT_INDEX, sn);
	return readSE0165B(SE0165B_GFP_RCMF_DAT);
}

/*
 * 设置E1时钟模式，
 * 	mode 0 主时钟
 * 		 1恢复时钟
 */
void setE1ClockMode(uint8 mode) {
	uint8 v = readSE0165B(SE0165B_E1_CFG);
	if( mode == 0 ) {
		//主时钟
		v &= ~(1<<1);
	}
	else {
		v |= (1<<1);
	}
	writeSE0165B(SE0165B_E1_CFG, v);
}

/*
 * 恢复时钟支路选择
 * 	e1port 0~7
 */
void setE1RecoverClockSouce(uint8 e1port) {
	uint8 v = readSE0165B(SE0165B_E1_CFG);
	v &= ~0xf;
	v |= (e1port & 0xf);
	writeSE0165B(SE0165B_E1_CFG2, v);
}


bool ifGFPSyncLOSS(void) {
	uint8 v = readSE0165B(SE0165B_GFP_STA);
	return (v & 3) != 2;
}

/*
 * 读取GFP告警
 * return
 * 	bit0 ack_tmo
 * 	bit1 LOA
 * 	bit2 GIDM
 * 	bit3 DELAYERR
 * 	bit4 MFIERR
 * 	bit5 SYNCLOSS
 * 	bit6 CRCERR
 * 	bit7 FCSERR
 */
uint8 getVCGAlarm(void) {
	uint8 tvcgad = readSE0165B(SE0165B_LCAS_TVCG_ALARM) & 1; //ack tmo
	uint8 rvcgad = readSE0165B(SE0165B_LCAS_RVCG_ALARM) & 0x1e; //mfi, delay gidm loa
	uint8 rgfpad = readSE0165B(SE0165B_GFP_ALARM);
	uint8 fcs = (rgfpad >> 5) & 1;
	uint8 syncloss = (rgfpad >> 1) & 1;
	uint8 crc = ((rgfpad & 0x18) == 0)?0:1;
	uint8 rtnalarm = tvcgad | rvcgad | fcs | syncloss | crc;
	return rtnalarm;
}

/*
 * 读取VCG Member告警
 * return
 * 	bit0 SQM
 * 	bit1 CRC
 * 	bit2 LOM2
 * 	bit3 LOM1
 */
uint8 getMemberAlarm(uint8 msn) {
	uint8 almdata = readSE0165B(SE0165B_LCAS_RTRIB_ALARM(msn));
	return (almdata & 0x03) | ((almdata >> 3) & 3);
}


void testChipSE0165B(void) _task_ tsk_test {
	xdata uint16 chipID = 0x0165;
	while(1) {
	    os_delete_task (tsk_test);            	//stop init task (no longer needed)
	}
}

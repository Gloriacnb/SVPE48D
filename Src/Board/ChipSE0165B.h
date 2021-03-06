/*
 * ChipSE0165B.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_BOARD_CHIPSE0165B_H_
#define SRC_BOARD_CHIPSE0165B_H_

#include "..\inc\type.h"

uint8 readSE0165B(uint16 regaddr);
void writeSE0165B(uint16 regaddr, uint8 regdata);
bool initSE0165B(void);
void sendByteSDO0(uint8 byte);
uint8 getByteSDO0(void);
int getE1Alarm(uint8 portN);
uint16 readMDIO(uint8 phy, uint8 phyreg);
bool writeMDIO(uint8 phy, uint8 phyreg, uint16 regData);

/*for CMF 管理通道*/
void writeTCMFData(uint8 sn, uint8 b);
void setTCMFLength(uint8 len);
bool ifTCMFOver(void);
void startTCMF(void);

bool ifRCMFReady(void);
uint8 getRCMFLength(void);
uint8 readRCMFData(uint8 sn);

/*for E1时钟*/
void setE1ClockMode(uint8 mode);
void setE1RecoverClockSouce(uint8 e1port);

/*for GFP ALARM*/
bool ifGFPSyncLOSS(void);

#define SE0165B_GLOBAL_ADDR_BASE								0x0000
#define SE0165B_E1_ADDR_BASE									0x0100
#define SE0165B_LCAS_ADDR_BASE									0x0200
#define SE0165B_GFP_ADDR_BASE									0x0300
#define SE0165B_ETH_ADDR_BASE									0x0400
#define SE0165B_HDLC_ADDR_BASE									0x0500

#define SE0165B_CHIPID			0x0165
#define SE0165B_E1_MAXNUM		8

/************全局寄存器*************/
#define SE0165B_GLOBAL_RSTLOOP1_REG								(SE0165B_GLOBAL_ADDR_BASE + 0x00)
#define SE0165B_GLOBAL_CHIP_IDH									(SE0165B_GLOBAL_ADDR_BASE + 0x02)
#define SE0165B_GLOBAL_CHIP_IDL									(SE0165B_GLOBAL_ADDR_BASE + 0x03)
#define SE0165B_GLOBAL_CHIP_VER									(SE0165B_GLOBAL_ADDR_BASE + 0x04)

#define SE0165B_GLOBAL_PHYADDR_REG								(SE0165B_GLOBAL_ADDR_BASE + 0x0D)
#define SE0165B_GLOBAL_PHYREGADDR_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x0E)
#define SE0165B_GLOBAL_MDIOCONTROL_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x0F)
#define SE0165B_GLOBAL_MDIOWDATH_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x10)
#define SE0165B_GLOBAL_MDIOWDATL_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x11)
#define SE0165B_GLOBAL_MDIORDATH_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x12)
#define SE0165B_GLOBAL_MDIORDATL_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x13)

#define MDIO_START	(1<<7)
#define MDIO_WRITE	(1<<6)
#define MDIO_READY	(1<<0)

#define SE0165B_GLOBAL_SDIO_OPT									(SE0165B_GLOBAL_ADDR_BASE + 0x6F)
#define SE0165B_GLOBAL_SDO0BYTE0_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x70)
#define SE0165B_GLOBAL_SDO0BYTE1_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x71)
#define SE0165B_GLOBAL_SDO0BYTE2_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x72)
#define SE0165B_GLOBAL_SDO0BYTE3_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x73)

#define SE0165B_GLOBAL_SDO1BYTE0_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x74)
#define SE0165B_GLOBAL_SDO1BYTE1_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x75)
#define SE0165B_GLOBAL_SDO1BYTE2_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x76)
#define SE0165B_GLOBAL_SDO1BYTE3_REG							(SE0165B_GLOBAL_ADDR_BASE + 0x77)

#define SE0165B_GLOBAL_SDIBYTE0_REG								(SE0165B_GLOBAL_ADDR_BASE + 0x7F)
#define SE0165B_GLOBAL_SDIBYTE1_REG								(SE0165B_GLOBAL_ADDR_BASE + 0x7E)
#define SE0165B_GLOBAL_SDIBYTE2_REG								(SE0165B_GLOBAL_ADDR_BASE + 0x7D)
#define SE0165B_GLOBAL_SDIBYTE3_REG								(SE0165B_GLOBAL_ADDR_BASE + 0x7C)

#define SE0165B_GLOBAL_WLOCK_REG								(SE0165B_GLOBAL_ADDR_BASE + 0xC0)

/***************LCAS寄存器****************/
#define SE0165B_LCAS_TVCG_CFG									(SE0165B_LCAS_ADDR_BASE + 0x10)
#define SE0165B_LCAS_RVCG_CFG									(SE0165B_LCAS_ADDR_BASE + 0x80)
#define SE0165B_LCAS_TTRIB_USE_SQ(n)							(SE0165B_LCAS_ADDR_BASE + 0x40 + (n) * 0x8)
#define SE0165B_LCAS_RTRIB_USE_SQ(n)							(SE0165B_LCAS_ADDR_BASE + 0xC0 + (n) * 0x8)


/***************E1支路寄存器***************/
#define SE0165B_E1_ALARM1(n)										(SE0165B_E1_ADDR_BASE + 0x00 + (n) * 0x10)
#define SE0165B_E1_ALARM2(n)										(SE0165B_E1_ADDR_BASE + 0x04 + (n) * 0x10)
#define SE0165B_E1_CFG											(SE0165B_E1_ADDR_BASE + 0xE1)
#define SE0165B_E1_CFG2											(SE0165B_E1_ADDR_BASE + 0xE2)

/***************GFP寄存器*****************/
#define SE0165B_GFP_RCMF_READY									(SE0165B_GFP_ADDR_BASE + 0x19)
#define SE0165B_GFP_RCMF_DAT_INDEX								(SE0165B_GFP_ADDR_BASE + 0x1A)
#define SE0165B_GFP_RCMF_DAT									(SE0165B_GFP_ADDR_BASE + 0x1B)
#define SE0165B_GFP_RCMF_DAT_LEN								(SE0165B_GFP_ADDR_BASE + 0x1C)

#define SE0165B_GFP_TCMF_CTRL									(SE0165B_GFP_ADDR_BASE + 0x0A)
#define SE0165B_GFP_TCMF_DAT_INDEX								(SE0165B_GFP_ADDR_BASE + 0x0C)
#define SE0165B_GFP_TCMF_DAT									(SE0165B_GFP_ADDR_BASE + 0x0D)
#define SE0165B_GFP_TCMF_DAT_LEN								(SE0165B_GFP_ADDR_BASE + 0x06)

#define SE0165B_GFP_STA											(SE0165B_GFP_ADDR_BASE + 0x11)
#define SE0165B_GFP_ALARM										(SE0165B_GFP_ADDR_BASE + 0x17)


void softReset(void);
uint16 readChipID(void);
void setDefault(void);

#endif /* SRC_BOARD_CHIPSE0165B_H_ */

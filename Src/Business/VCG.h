/*
 * VCG.h
 *
 *  Created on: 2017年9月13日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_VCG_H_
#define SRC_BUSINESS_VCG_H_

#include "../Alarm/AlmSrcVCG.h"
#include "../Board/BoardConfig.h"

typedef struct VCG_ALM_ATTR_DATA_T{
	uint8 attr[VCG_TYPE_SIZE];
	uint8 tGfp;		//bit7:5 TPTI,bit4 TPFI,bit3:0 TEXI
	uint8 gfp_tupi;
	uint8 gfp_tcid;
	uint8 gfp_crb;	//bit1 CHEC扰码使能，bit0 DATA扰码使能
	uint8 rGfp;		//bit7:6 RPFIMOD
					//0：PFI 来自接收的 THEC，根据接收的 PFI 进行 FCS 校验。
					//1：PFI 来自接收的 THEC，根据接收的 PFI 处理 FCS 字段，但不进行 FCS校验。
					//2： PFI 强制为 1；
					//3： PFI 强制为 0
					//Bit5 CHEC单bit纠错使能(1)
					//Bit1 接收 CHEC 解扰码使能(1)
					//Bit0 接收净荷区解扰码使能(1)
	uint8 tVcg;		//bit7 TSQ MOD
					//1由 MCU 设置的 cTTRIB_SQ 值决定。
					//0 根据VCAT/LCAS 协议自动生成
					//Bit1 非LCAS（0）LCAS（1）
					//Bit0 非VCAT(0)VCAT(1)
	uint8 rVcg;		// bit4屏蔽CRC校验(1)
					//Bit3 TSF告警触发LCAS(1)
					//Bit2 SQ来源
					//0：接收
					//1：MCU 配置 cTRIB_SQ
					//Bit1 非LCAS（0）LCAS（1）
					//Bit0 非VCAT(0)VCAT(1)

}VCG_CFG_DATA;

typedef struct ALL_VCG_CONFIG {
	VCG_CFG_DATA vcg[BD_VCG_NUM];
	uint8 crc;
}VCG_CFG;

void initVCGConfig(void);

void readVCGConfigData(void);
bool saveVCGCfg(void);

VCG_CFG_DATA* getVCGConfigStruct(void);

uint8 getVCGConfigData(uint8* buf);
bool setVCGConfig(uint8* buf, uint8 len);

#endif /* SRC_BUSINESS_VCG_H_ */

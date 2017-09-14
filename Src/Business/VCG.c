/*
 * VCG.c
 *
 *  Created on: 2017年9月13日
 *      Author: Administrator
 */

#include "../Business/VCG.h"
#include "../inc/taskID.h"
#include "../Business/ConfigData.h"
#include "../inc/ErrCode.h"
#include "../Board/ChipSE0165B.h"

static xdata VCG_CFG CFG_DATA = {0};

void initVCGConfig(void) {
	readVCGConfigData();
}


void readVCGConfigData(void) {
	uint8 i,j;
	readConfig(VCG_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(VCG_CFG));
	if( CFG_DATA.crc != 0x5A ) {
		for (i = 0; i < BD_VCG_NUM; ++i) {
			for (j = 0; j < VCG_TYPE_SIZE; ++j) {
				CFG_DATA.vcg[i].attr[j] = 0x0A;
			}
		}
		saveVCGCfg();
	}
}
bool saveVCGCfg(void) {
	CFG_DATA.crc = 0x5A;
	saveConfig(VCG_CFG_SECTOR, (uint8*)&CFG_DATA, sizeof(VCG_CFG));
	return true;
}

VCG_CFG_DATA* getVCGConfigStruct(void) {
	return CFG_DATA.vcg;
}

/*
 * 读VCG配置数据
 * 	buf 输出参数
 * 	return 配置数据字节数
 */
uint8 getVCGConfigData(uint8* buf) {
	if( buf == 0 ) {
		return ERR_NULLPOINTER;
	}
	buf[0] = CFG_DATA.vcg.tGfp;
	buf[1] = CFG_DATA.vcg.gfp_tupi;
	buf[2] = CFG_DATA.vcg.gfp_tcid;
	buf[3] = CFG_DATA.vcg.gfp_crb;
	buf[4] = CFG_DATA.vcg.rGfp;
	buf[5] = CFG_DATA.vcg.tVcg;
	buf[6] = CFG_DATA.vcg.rVcg;
	return 7;
}

/*
 * 配置VCG
 */
bool setVCGConfig(uint8* buf, uint8 len) {
	if( (buf == 0) || (len < 7) ) {
		return false;
	}
	writeSE0165B(SE0165B_GFP_T_CFG, buf[0]);
	writeSE0165B(SE0165B_GFP_T_UPI, buf[1]);
	writeSE0165B(SE0165B_GFP_T_CID, buf[2]);
	writeSE0165B(SE0165B_GFP_T_CRB, buf[3]);
	writeSE0165B(SE0165B_GFP_R_CFG, buf[4]);
	writeSE0165B(SE0165B_LCAS_TVCG_CFG, buf[5]);
	writeSE0165B(SE0165B_LCAS_RVCG_CFG, buf[6]);
	CFG_DATA.vcg.tGfp     = buf[0];
	CFG_DATA.vcg.gfp_tupi = buf[1];
	CFG_DATA.vcg.gfp_tcid = buf[2];
	CFG_DATA.vcg.gfp_crb  = buf[3];
	CFG_DATA.vcg.rGfp     = buf[4];
	CFG_DATA.vcg.tVcg     = buf[5];
	CFG_DATA.vcg.rVcg     = buf[6];
	return saveVCGCfg();
}

/*
 * ConfigModule.c
 *
 *  Created on: 2017年9月13日
 *      Author: Administrator
 */

#include "../Business/ConfigModule.h"
#include "../Business/E1Port.h"
#include "../Business/VCG.h"
#include "../Business/Member.h"
#include "../Business/EthPort.h"

void initConfigModule(void) {
	initE1PortConfig();
	initVCGConfig();
	initMembConfig();
	initEthPortConfig();
}

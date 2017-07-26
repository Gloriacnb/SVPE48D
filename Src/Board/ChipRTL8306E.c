/*
 * ChipRTL8306E.c
 *
 *  Created on: 2017年7月26日
 *      Author: Administrator
 */

#include "..\Board\ChipRTL8306E.h"
#include "..\RTLLib\rtk_api_ext.h"

bool initRTL8306E(void) {
	if( rtk_switch_init() != RT_ERR_OK ) {
		return false;
	}
	return true;
}


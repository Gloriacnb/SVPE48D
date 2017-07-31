/*
 * Device.c
 *
 *  Created on: 2017年7月30日
 *      Author: Administrator
 */

#include "..\Business\Device.h"

static const uint8 DEVICE_TYPE[] = {0x30, 0x44};
static const uint8 HARDWARE_VER[] = {1,0};
static const uint8 SOFTWARE_VER[] = {1,0,0};

uint8 getFactoryInfo(uint8* info) {
	uint8 dataLen = 0;
	char i;
	if( info == 0 ) {
		return 0;
	}
	for (i = 0; i < sizeof(DEVICE_TYPE); ++i) {
		info[dataLen++] = DEVICE_TYPE[i];
	}
	for (i = 0; i < sizeof(HARDWARE_VER); ++i) {
		info[dataLen++] = HARDWARE_VER[i];
	}
	for (i = 0; i < sizeof(SOFTWARE_VER); ++i) {
		info[dataLen++] = SOFTWARE_VER[i];
	}
	return dataLen;
}


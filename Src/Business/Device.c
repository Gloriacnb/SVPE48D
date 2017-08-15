/*
 * Device.c
 *
 *  Created on: 2017年7月30日
 *      Author: Administrator
 */


#include "../Business/Device.h"
#include "../Business/ConfigData.h"
#include "../inc/ErrCode.h"

static const uint8 DEVICE_TYPE[] = {0x30, 0x44};
static const uint8 HARDWARE_VER[] = {3,2};
static const uint8 SOFTWARE_VER[] = {1,0,2};

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
	readConfig(DEV_ATTR_SECTOR, &info[dataLen], FACTORY_INFO_BYTES);
	return dataLen + FACTORY_INFO_BYTES;
}

uint8 setFactoryInfo(uint8* d, uint8 len) {
	if( d == 0 || len < FACTORY_INFO_BYTES ) {
		return ERR_INPUT;
	}
	return saveConfig(DEV_ATTR_SECTOR, d, len);
}

uint16 getSerialNumber(void) {
	uint8 info[FACTORY_INFO_BYTES];
	readConfig(DEV_ATTR_SECTOR, info, FACTORY_INFO_BYTES);
	return (info[FACTORY_INFO_BYTES-2] << 8) | info[FACTORY_INFO_BYTES-1];
}

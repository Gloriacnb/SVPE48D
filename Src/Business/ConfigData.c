/*
 * ConfigData.c
 *
 *  Created on: 2017年7月31日
 *      Author: Administrator
 */
#include "../Business/ConfigData.h"
#include "../STCLib/EEPROM.h"
#include "../inc/ErrCode.h"

uint8 readConfig(uint16 sec, uint8* d, uint16 len) {
	EEPROM_read_n(EEADDR(sec), d, len);
	return ERR_NONE;
}

uint8 saveConfig(uint16 sec, uint8* d, uint16 len) {
	EEPROM_SectorErase(EEADDR(sec));
	EEPROM_write_n(EEADDR(sec), d, len);
	return ERR_NONE;
}

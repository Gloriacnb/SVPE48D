/*
 * ConfigData.h
 *
 *  Created on: 2017年7月31日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_CONFIGDATA_H_
#define SRC_BUSINESS_CONFIGDATA_H_

#include "../inc/type.h"

#define DEV_ATTR_SECTOR		0

#define EEADDR(n)		((n)<<9)

uint8 readConfig(uint16 sec, uint8* d, uint16 len);
uint8 saveConfig(uint16 sec, uint8* d, uint16 len);

#endif /* SRC_BUSINESS_CONFIGDATA_H_ */

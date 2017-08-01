/*
 * Device.h
 *
 *  Created on: 2017年7月30日
 *      Author: Administrator
 */

#ifndef SRC_BUSINESS_DEVICE_H_
#define SRC_BUSINESS_DEVICE_H_

#include "..\inc\type.h"
#define Production_date_bytes	4
#define Exfactory_date_bytes	4
#define Warranty_date_bytes		4
#define LastRepair_date_bytes	4
#define Device_serialnumber_bytes 2

#define FACTORY_INFO_BYTES	(Production_date_bytes +\
							 Exfactory_date_bytes  +\
							 Warranty_date_bytes   +\
							 LastRepair_date_bytes +\
							 Device_serialnumber_bytes)

uint8 getFactoryInfo(uint8* info);
uint8 setFactoryInfo(uint8* d, uint8 len);
uint16 getSerialNumber(void);
#endif /* SRC_BUSINESS_DEVICE_H_ */

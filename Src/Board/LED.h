/*
 * LED.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_BOARD_LED_H_
#define SRC_BOARD_LED_H_

#include "..\inc\type.h"

void initLEDs(void);

void turnOnE1LEDs(void);
void turnOffE1LEDs(void);
void refreshE1LEDs(uint8 ledSta);
#endif /* SRC_BOARD_LED_H_ */

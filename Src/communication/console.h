/*
 * console.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_BOARD_CONSOLE_H_
#define SRC_BOARD_CONSOLE_H_

#include "..\inc\type.h"
#include "..\communication\command.h"

void initConsole(void);
void consoleSendFrame(CMD_FRAME* f);
#endif /* SRC_BOARD_CONSOLE_H_ */

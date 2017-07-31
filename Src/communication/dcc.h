/*
 * dcc.h
 *
 *  Created on: 2017年7月29日
 *      Author: Administrator
 */

#ifndef SRC_COMMUNICATION_DCC_H_
#define SRC_COMMUNICATION_DCC_H_

#include "..\communication\command.h"

void initDCC(void);
void dccSendFrame(CMD_FRAME* f);
#endif /* SRC_COMMUNICATION_DCC_H_ */

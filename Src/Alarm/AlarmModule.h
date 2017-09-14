/*
 * AlarmModule.h
 *
 *  Created on: 2017年9月4日
 *      Author: Administrator
 */

#ifndef SRC_ALARM_ALARMMODULE_H_
#define SRC_ALARM_ALARMMODULE_H_

#include "../Alarm/AlarmDefine.h"

void runAlarmModule(void);	//初始化并运行告警模块，上层调用初始化入口


/*告警属性配置接口*/
uint8 getAlarmAttribute(uint8 src, uint8 type);
bool setAlarmAttribute(uint8 src, uint8 type, uint8 newattr);

void reportAlarm(uint8 sid, uint8 tid, bool raise, uint8 level);

ALM_SRC* getAlarmSourceBySid(uint8 sid);
#endif /* SRC_ALARM_ALARMMODULE_H_ */

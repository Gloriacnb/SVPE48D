/*
 * taskID.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_INC_TASKID_H_
#define SRC_INC_TASKID_H_

#ifdef IN_KEIL
#define tsk_init	0
#define tsk_run		1
#define tsk_led_e1  2
#define tsk_test	3
#else
#define _task_
#define tsk_init
#define tsk_run
#define tsk_led_e1
#define tsk_test
#endif



#endif /* SRC_INC_TASKID_H_ */

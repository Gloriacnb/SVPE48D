/*
 * taskID.h
 *
 *  Created on: 2017年7月20日
 *      Author: Administrator
 */

#ifndef SRC_INC_TASKID_H_
#define SRC_INC_TASKID_H_

#ifdef IN_KEIL
#define tsk_init		0
#define tsk_run			1
#define tsk_led_e1  	2
#define tsk_test		3
#define tsk_console_rcv	4
#define tsk_dcc_rcv		5
#else
#define _task_
#define tsk_init
#define tsk_run
#define tsk_led_e1
#define tsk_test
#define tsk_console_rcv
#define tsk_dcc_rcv

#define idata
#define xdata
#define sbit
#define bdata
#endif



#endif /* SRC_INC_TASKID_H_ */

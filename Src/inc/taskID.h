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
#define tsk_e1_rclock	6
#define tsk_loop_detect 7
#define tsk_send_loop	8
#else
#define _task_
#define tsk_init
#define tsk_run
#define tsk_led_e1
#define tsk_test
#define tsk_console_rcv
#define tsk_dcc_rcv
#define tsk_e1_rclock
#define tsk_loop_detect
#define tsk_send_loop

#define idata
#define xdata
#define sbit
#define bdata
#define bit
#define data
#endif



#endif /* SRC_INC_TASKID_H_ */

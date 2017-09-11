/*
 * main.c
 *
 *  Created on: 2017年7月19日
 *      Author: SHQP
 */
#include "main.h"
#include "Board\BoardSVPE48D.h"
#include "inc\taskID.h"
#include <RTX51TNY.H>
#include <stdio.h>

/******************************************************************************/
/*        Task 0 'INIT': Initialize                                           */
/******************************************************************************/
void init (void) _task_ tsk_init  {       /* program execution starts here        */

	boardInit();							//硬件初始化
    os_delete_task (tsk_init);            	//stop init task (no longer needed)
}



/*
 * 为了去除编辑页面警告，在51-tiny中不需要main函数
 */

#ifndef IN_KEIL
int main(void) {
	init();
}
#endif


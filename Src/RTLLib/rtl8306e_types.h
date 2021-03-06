/*
 * Copyright (C) 2009 Realtek Semiconductor Corp. 
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated, 
 * modified or distributed under the authorized license from Realtek. 
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER 
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED. 
 *
 * $Revision: 10083 $
 * $Date: 2010-06-07 11:18:41 +0800 (星期一, 07 六月 2010) $
 *
 * Purpose : RTL8306E switch API varable type declaration
 * Feature : 
 *
 */
 
 
#ifndef __RTL8306E_TYPES_H__
#define __RTL8306E_TYPES_H__

#include "..\inc\type.h"
#define EMBEDDED_SUPPORT    
//#ifndef _RTL_TYPES_H
////typedef unsigned long long uint64;
////typedef long long int64;
//typedef unsigned long uint32;
//typedef long int32;
//typedef unsigned short uint16;
//typedef short int16;
//typedef unsigned char uint8;
//typedef char int8;
//#endif

typedef int16                   rtk_api_ret_t;
typedef int16                   ret_t;
//typedef uint64                  rtk_u_long_t;

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif


#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCESS
#define SUCCESS 0
#endif
#ifndef FAILED
#define FAILED -1
#endif

#define rtlglue_printf printf

#endif

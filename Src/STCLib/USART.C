
#include "USART.h"
#include <RTX51TNY.H>

#define  OLEN  32                      /* size of serial transmission buffer   */
unsigned char  ostart;                /* transmission buffer start index      */
unsigned char  oend;                  /* transmission buffer end index        */
idata    char  outbuf[OLEN];          /* storage for transmission buffer      */
unsigned char  otask = 0xff;          /* task number of output task           */

#define  ILEN  32                     /* size of serial receiving buffer      */
unsigned char  istart;                /* receiving buffer start index         */
unsigned char  iend;                  /* receiving buffer end index           */
idata    char  inbuf[ILEN];           /* storage for receiving buffer         */
unsigned char  itask = 0xff;          /* task number of output task           */

static bit   sendfull;                /* flag: marks transmit buffer full     */
static bit   sendactive;              /* flag: marks transmitter active       */
//static bit   sendstop;                /* flag: marks XOFF character           */

u8 USART_Configuration(u8 UARTx, COMx_InitDefine *COMx)
{
	u8	i;
	u16	j;
	
	sendfull = 0;
	sendactive = 0;
//	sendstop = 0;

	if(UARTx == USART1)
	{
		ostart    	= 0;
		oend	   	= 0;
		otask  		= 0xff;
		istart     	= 0;
		iend	 	= 0;
		itask    	= 0xff;
		for(i=0; i<OLEN; i++)	outbuf[i] = 0;
		for(i=0; i<ILEN; i++)	inbuf[i] = 0;

		if(COMx->UART_Mode > UART_9bit_BRTx)	return 2;	//模式错误
		if(COMx->UART_Polity == PolityHigh)		PS = 1;	//高优先级中断
		else									PS = 0;	//低优先级中断
		SCON = (SCON & 0x3f) | COMx->UART_Mode;
		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;

			if(COMx->UART_BRT_Use == BRT_Timer1)
			{
				TR1 = 0;
				AUXR &= ~0x01;		//S1 BRT Use Timer1;
				TMOD &= ~(1<<6);	//Timer1 set As Timer
				TMOD &= ~0x30;		//Timer1_16bitAutoReload;
				AUXR |=  (1<<6);	//Timer1 set as 1T mode
				TH1 = (u8)(j>>8);
				TL1 = (u8)j;
				ET1 = 0;	//禁止中断
				TMOD &= ~0x40;	//定时
				INT_CLKO &= ~0x02;	//不输出时钟
				TR1  = 1;
			}
			else if(COMx->UART_BRT_Use == BRT_Timer2)
			{
				AUXR &= ~(1<<4);	//Timer stop
				AUXR |= 0x01;		//S1 BRT Use Timer2;
				AUXR &= ~(1<<3);	//Timer2 set As Timer
				AUXR |=  (1<<2);	//Timer2 set as 1T mode
				TH2 = (u8)(j>>8);
				TL2 = (u8)j;
				IE2  &= ~(1<<2);	//禁止中断
				AUXR &= ~(1<<3);	//定时
				AUXR |=  (1<<4);	//Timer run enable
			}
			else return 2;	//错误
		}
		else if(COMx->UART_Mode == UART_ShiftRight)
		{
			if(COMx->BaudRateDouble == ENABLE)	AUXR |=  (1<<5);	//固定波特率SysClk/2
			else								AUXR &= ~(1<<5);	//固定波特率SysClk/12
		}
		else if(COMx->UART_Mode == UART_9bit)	//固定波特率SysClk*2^SMOD/64
		{
			if(COMx->BaudRateDouble == ENABLE)	PCON |=  (1<<7);	//固定波特率SysClk/32
			else								PCON &= ~(1<<7);	//固定波特率SysClk/64
		}
		if(COMx->UART_Interrupt == ENABLE)	ES = 1;	//允许中断
		else								ES = 0;	//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	REN = 1;	//允许接收
		else								REN = 0;	//禁止接收
		P_SW1 = (P_SW1 & 0x3f) | (COMx->UART_P_SW & 0xc0);	//切换IO
		if(COMx->UART_RXD_TXD_Short == ENABLE)	PCON2 |=  (1<<4);	//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
		else									PCON2 &= ~(1<<4);
		return	0;
	}

/*
	if(UARTx == USART2)
	{
		COM2.id = 1;
		COM2.ostart    	= 0;
		COM2.oend	   	= 0;
		COM2.otask  	= 0xff;
		COM2.istart     = 0;
		COM2.iend	 	= 0;
		COM2.itask    	= 0xff;
		for(i=0; i<OLEN; i++)	COM2.outbuf[i] = 0;
		for(i=0; i<ILEN; i++)	COM2.inbuf[i] = 0;

		if((COMx->UART_Mode == UART_9bit_BRTx) ||(COMx->UART_Mode == UART_8bit_BRTx))	//可变波特率
		{
			if(COMx->UART_Polity == PolityHigh)		IP2 |=  1;	//高优先级中断
			else									IP2 &= ~1;	//低优先级中断
			if(COMx->UART_Mode == UART_9bit_BRTx)	S2CON |=  (1<<7);	//9bit
			else									S2CON &= ~(1<<7);	//8bit
			j = (MAIN_Fosc / 4) / COMx->UART_BaudRate;	//按1T计算
			if(j >= 65536UL)	return 2;	//错误
			j = 65536UL - j;
			AUXR &= ~(1<<4);	//Timer stop
			AUXR &= ~(1<<3);	//Timer2 set As Timer
			AUXR |=  (1<<2);	//Timer2 set as 1T mode
			TH2 = (u8)(j>>8);
			TL2 = (u8)j;
			IE2  &= ~(1<<2);	//禁止中断
			AUXR |=  (1<<4);	//Timer run enable
		}
		else	return 2;	//模式错误
		if(COMx->UART_Interrupt == ENABLE)	IE2   |=  1;		//允许中断
		else								IE2   &= ~1;		//禁止中断
		if(COMx->UART_RxEnable == ENABLE)	S2CON |=  (1<<4);	//允许接收
		else								S2CON &= ~(1<<4);	//禁止接收
		P_SW2 = (P_SW2 & ~1) | (COMx->UART_P_SW & 0x01);	//切换IO
	}
*/
		return	0;
}


/*************** 装载串口发送缓冲 *******************************/

void putBuf1(char c)	//写入发送缓冲，指针+1
{
	if (!sendfull) { 								/* transmit only if buffer not full     */
		ES = 0; 									/* disable serial interrupt             */
		if (!sendactive /*&& !sendstop*/) { 		/* if transmitter not active:           */
			sendactive = 1; 						/* transfer the first character direct  */
			SBUF = c; 								/* to SBUF to start transmission        */
		} else { 									/* otherwize:                           */
			outbuf[oend++ & (OLEN - 1)] = c; 		/* transfer char to transmission buffer */
			if (((oend ^ ostart) & (OLEN - 1)) == 0)
				sendfull = 1;
		} 											/* set flag if buffer is full           */
		ES = 1; 									/* enable serial interrupt              */
	}
}

/********************* UART1中断函数************************/
void UART1_int (void) interrupt UART1_VECTOR using 2
{
	u8 c;
	if (RI) { 										/* if receiver interrupt                 */
		c = SBUF; 									/* read character                        */
		RI = 0; 									/* clear interrupt request flag          */
		if (istart + ILEN != iend) {
			inbuf[iend++ & (ILEN-1)] = c;
		}
													/* if task waiting: signal ready         */
		if (itask != 0xFF) isr_send_signal (itask);
	}

	if (TI ) { 										/* if transmitter interrupt              */
		TI = 0; 									/* clear interrupt request flag          */
		if (ostart != oend) { 						/* if characters in buffer and           */
			SBUF = outbuf[ostart++ & (OLEN-1)]; 	/* transmit character        */
			sendfull = 0; 							/* clear 'sendfull' flag                 */
													/* if task waiting: signal ready         */
			if (otask != 0xFF) {
				isr_send_signal(otask);
			}
		}
		else {
			sendactive = 0; 						/* if all transmitted clear 'sendactive' */
		}
	}
}


/*
 * functions for C-Lib
 */
/******************************************************************************/
/*       putchar:  interrupt controlled putchar function                      */
/******************************************************************************/
char putchar(char c) {
	while (sendfull) { 					/* wait for transmission buffer empty   */
		otask = os_running_task_id(); 	/* set output task number               */
		os_wait(K_SIG, 0, 0); 			/* RTX-51 call: wait for signal         */
		otask = 0xff; 					/* clear output task number             */
	}
	putBuf1(c); 						/* send character                       */
	return (c); 						/* return character: ANSI requirement   */
}


/******************************************************************************/
/*       _getkey:  interrupt controlled _getkey                               */
/******************************************************************************/
char _getkey(void) {
	while (iend == istart) {
		itask = os_running_task_id(); 	/* set input task number                */
		os_wait(K_SIG, 0, 0); 			/* RTX-51 call: wait for signal         */
		itask = 0xff; 					/* clear input task number              */
	}
	return (inbuf[istart++ & (ILEN - 1)]);
}


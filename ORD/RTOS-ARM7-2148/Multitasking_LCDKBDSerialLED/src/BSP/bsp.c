/*
*********************************************************************************************************
*                                               Philips LPC214x
*                                 LPC2148 Kick Start Card Board Support Package
*
*                                    (c) Copyright 2005, Micrium, Weston, FL
*                                              All Rights Reserved
*
*
* File : BSP.C
* By   : Jean J. Labrosse
*********************************************************************************************************
*/

#define  BSP_GLOBALS
#include "includes.h"
#define  BSP_DEBUG 1
#include "lcd.h"
#include "UART0.h"
#include "KBD.h"
#include <stdio.h>
/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define  BSP_UNDEF_INSTRUCTION_VECTOR_ADDR   (*(INT32U *)0x00000004L)
#define  BSP_SWI_VECTOR_ADDR                 (*(INT32U *)0x00000008L)
#define  BSP_PREFETCH_ABORT_VECTOR_ADDR      (*(INT32U *)0x0000000CL)
#define  BSP_DATA_ABORT_VECTOR_ADDR          (*(INT32U *)0x00000010L)
#define  BSP_IRQ_VECTOR_ADDR                 (*(INT32U *)0x00000018L)
#define  BSP_FIQ_VECTOR_ADDR                 (*(INT32U *)0x0000001CL)

#define  BSP_IRQ_ISR_ADDR                    (*(INT32U *)0x00000038L)
#define  BSP_FIQ_ISR_ADDR                    (*(INT32U *)0x0000003CL)


/* For 4x4 Matrix Keyboard	*/
#define RET0	0x00010000	// P1.16
#define RET1	0x00020000	// P1.17
#define RET2	0x00040000	// P1.18
#define RET3	0x00080000	// P1.19

#define	ALL_RET	(RET0|RET1|RET2|RET3)

#define SCAN0	0x00100000	// P1.20
#define SCAN1	0x00200000	// P1.21
#define SCAN2	0x00400000	// P1.22
#define SCAN3	0x00800000	// P1.23

const uint32 KBD_u32scanport1[4]={SCAN0,SCAN1,SCAN2,SCAN3};

const uint32 KBD_u32retport[4]={RET0,RET1,RET2,RET3};

#define CRYSTAL_FREQUENCY_IN_HZ 12000000
#define PLL_MULTIPLIER	1
#define DEL_LOOPS	CRYSTAL_FREQUENCY_IN_HZ*PLL_MULTIPLIER/500000

int32 KBD_i32keydown;

int32 KBD_i32the_sc ;
/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/

typedef  void (*PFNCT)(void);

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

        INT32U      Tmr_ReloadCnts;
/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/


static  void  Tmr_TickInit(void);

/*
*********************************************************************************************************
*                                      Get the CPU Clock Frequency
*
* Description: This function reads CPU registers to determine the CPU clock frequency of the chip.
*********************************************************************************************************
*/

INT32U  BSP_CPU_ClkFreq (void)
{
    INT32U  msel;
    INT32U  cpu_clk_freq;


    msel         = (INT32U)(PLLCFG & 0x1F);
    cpu_clk_freq = CPU_OSC_FREQ * (msel + 1);
    return (cpu_clk_freq);
}

/*
*********************************************************************************************************
*                                   Get the Peripheral Clock Frequency
*
* Description: This function reads CPU registers to determine the peripheral clock frequency of the chip.
*********************************************************************************************************
*/

INT32U  BSP_CPU_ClkFreqPeripheral (void)
{
    INT32U  msel;
    INT32U  vpbdiv;
    INT32U  cpu_clk_freq;
    INT32U  cpu_peripheral_clk_freq;


    msel         = (INT32U)(PLLCFG   & 0x1F);
    cpu_clk_freq = CPU_OSC_FREQ * (msel + 1);

    vpbdiv       = (INT32U)(VPBDIV & 0x03);
    switch (vpbdiv) {
        case 0:
             cpu_peripheral_clk_freq = cpu_clk_freq / 4;
             break;

        case 1:
             cpu_peripheral_clk_freq = cpu_clk_freq;
             break;

        case 2:
             cpu_peripheral_clk_freq = cpu_clk_freq / 2;
             break;

        default:
             cpu_peripheral_clk_freq = cpu_clk_freq / 4;
             break;
    }
    return (cpu_peripheral_clk_freq);
}

/*
*********************************************************************************************************
*                                         BSP INITIALIZATION
*
* Description : This function should be called by your application code before you make use of any of the
*               functions found in this module.
*
* Arguments   : none
*********************************************************************************************************
*/

void  BSP_Init (void)
{
   	MEMMAP                            = 2;                      /* Remap 64 bytes of int. RAM to 0x00  */

    BSP_IRQ_VECTOR_ADDR               = 0xE59FF018;             /* LDR PC,[PC,#0x18] instruction       */
	BSP_IRQ_ISR_ADDR                  = (INT32U)OS_CPU_IRQ_ISR; /* IRQ exception vector address        */

    BSP_FIQ_VECTOR_ADDR               = 0xE59FF018;             /* LDR PC,[PC,#0x18] instruction       */
    BSP_FIQ_ISR_ADDR                  = (INT32U)OS_CPU_FIQ_ISR; /* FIQ exception vector address        */

    Tmr_TickInit();                               /* Initialize the uC/OS-II tick interrupt            */
    
	InitUart0();
    
    LcdInit ();

}

/*
*********************************************************************************************************
*                                           IRQ ISR HANDLER
*
* Description : This function is called by OS_CPU_IRQ_ISR() to determine the source of the interrupt
*               and process it accordingly.
*
* Arguments   : none
*********************************************************************************************************
*/

void  OS_CPU_IRQ_ISR_Handler (void)
{
    PFNCT  pfnct;


    pfnct = (PFNCT)VICVectAddr;                 /* Read the interrupt vector from the VIC               */
    while (pfnct != (PFNCT)0) {                 /* Make sure we don't have a NULL pointer               */
      (*pfnct)();                               /* Execute the ISR for the interrupting device          */
        pfnct       = (PFNCT)VICVectAddr;       /* Read the interrupt vector from the VIC               */
    }
}


/*
*********************************************************************************************************
*                                           FIQ ISR HANDLER
*
* Description : This function is called by OS_CPU_FIQ_ISR() to determine the source of the interrupt
*               and process it accordingly.
*
* Arguments   : none
*********************************************************************************************************
*/

void  OS_CPU_FIQ_ISR_Handler (void)
{
    PFNCT  pfnct;


    pfnct = (PFNCT)VICVectAddr;                 /* Read the interrupt vector from the VIC               */
    while (pfnct != (PFNCT)0) {                 /* Make sure we don't have a NULL pointer               */
      (*pfnct)();                               /* Execute the ISR for the interrupting device          */
        pfnct       = (PFNCT)VICVectAddr;       /* Read the interrupt vector from the VIC               */
    }
}



/*
*********************************************************************************************************
*                                       TICKER INITIALIZATION
*
* Description : This function is called to initialize uC/OS-II's tick source (typically a timer generating
*               interrupts every 1 to 100 mS).
*
* Arguments   : none
*
* Note(s)     : 1) The timer is setup for output compare mode BUT 'MUST' also 'freerun' so that the timer
*                  count goes from 0x00000000 to 0xFFFFFFFF to ALSO be able to read the free running count.
*                  The reason this is needed is because we use the free-running count in uC/OS-View.
*********************************************************************************************************
*/

static  void  Tmr_TickInit (void)
{
    INT32U  peripheral_clk_freq;


                                                 /* VIC TIMER #0 Initialization                        */
    VICIntSelect &= ~(1 << VIC_TIMER0);          /* Enable interrupts                                  */
    VICVectAddr2  = (INT32U)Tmr_TickISR_Handler; /* Set the vector address                             */
    VICVectCntl2  = 0x20 | VIC_TIMER0;           /* Enable vectored interrupts                         */
    VICIntEnable  =  (1 << VIC_TIMER0);          /* Enable Interrupts                                  */

    peripheral_clk_freq = BSP_CPU_ClkFreqPeripheral();
    Tmr_ReloadCnts      = peripheral_clk_freq / OS_TICKS_PER_SEC;
    T0TCR               = 0;                     /* Disable timer 0.                                   */
    T0PC                = 0;                     /* Prescaler is set to no division.                   */
#if BSP_DEBUG == 0
    T0MR0               = T0TC + Tmr_ReloadCnts;
    T0MCR               = 1;                     /* Interrupt on MR0 (match register 0).               */
#else
    T0MR0               = Tmr_ReloadCnts;
    T0MCR               = 3;                     /* Interrupt on MR0 (reset TC)                        */
#endif
    T0CCR               = 0;                     /* Capture is disabled.                               */
    T0EMR               = 0;                     /* No external match output.                          */
    T0TCR               = 1;                     /* Enable timer 0                                     */
}


/*
*********************************************************************************************************
*                                         TIMER #0 IRQ HANDLER
*
* Description : This function handles the timer interrupt that is used to generate TICKs for uC/OS-II.
*
* Arguments   : none
*
* Note(s)     : 1) The timer is 'reloaded' with the count at compare + the time for the next interrupt.
*                  Since we are using 'unsigned' integer math, overflows are irrelevant.
*********************************************************************************************************
*/

void  Tmr_TickISR_Handler (void)
{
    T0IR        = 0xFF;                          /* Clear timer #0 interrupt                           */
                                                 /* Reload 'relative' to current interrupt time        */
#if BSP_DEBUG == 0
    T0MR0      += Tmr_ReloadCnts;
#endif
    VICVectAddr = 0;
    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
}

/*
*********************************************************************************************************
*                                         UART0
*********************************************************************************************************
*/
#define DESIRED_BAUDRATE 19200

#define CRYSTAL_FREQUENCY_IN_HZ 12000000
#define PCLK CRYSTAL_FREQUENCY_IN_HZ	/* since VPBDIV=0x01	*/
#define DIVISOR (PCLK/(16*DESIRED_BAUDRATE))

void InitUart0(void)
{
	PINSEL0 |= 0x00000005 ;
/*	U0LCR: UART0 Line Control Register
	0x83: enable Divisor Latch access, set 8-bit word length,
	1 stop bit, no parity, disable break transmission		*/
	U0LCR=0x83;
	
/*	VPBDIV: VPB bus clock divider
	0x01: PCLK = processor clock		*/
	VPBDIV=0x01;
	
/*	U0DLL: UART0 Divisor Latch (LSB)	*/
	U0DLL=DIVISOR&0xFF;
	
/*	U0DLM: UART0 Divisor Latch (MSB)	*/
	U0DLM=DIVISOR>>8;
	
/*	U0LCR: UART0 Line Control Register
	0x03: same as above, but disable Divisor Latch access	*/
	U0LCR=0x03 ;
	
/*	U0FCR: UART0 FIFO Control Register
	0x05: Clear Tx FIFO and enable Rx and Tx FIFOs		*/
	U0FCR=0x05 ;
}

char uart_putc(char ch)
{
	if (ch=='\n')
	{
		//wait until Transmit Holding Register is empty
		while (!(U0LSR&0x20)) {}
		
		//then store to Transmit Holding Register
		U0THR='\r';
	}
	//wait until Transmit Holding Register is empty
	while (!(U0LSR&0x20)) {}
	
	//then store to Transmit Holding Register
	U0THR=ch;
	
	return ch;
}

char uart_getc(void)
{
	char ch;
	
	//wait until there's a character to be read
	while (!(U0LSR&0x01)) {}

	//then read from the Receiver Buffer Register
	ch=U0RBR;
	return ch;
}


/*
	This file contains LCD related sub-routines for EDU-2148 Trainer Kit.
	LCD connections:
	P1.25	P1.24	P0.30	P0.22	P0.17	P0.15
	RS		EN		D7		D6		D5		D4
	4 bit interface is used.

*/

void SmallDelay (void)
{

	int i;
	for(i=0;i<100;i++);	
}

void LcdCmd1 (unsigned char cmd)
{

	if (cmd & 0x01)
		IOSET0 = (1<<15);
	else
		IOCLR0 = (1<<15);
		

	if (cmd & 0x02)
		IOSET0 = (1<<17);
	else
		IOCLR0 = (1<<17);
		

	if (cmd & 0x04)
		IOSET0 = (1<<22);
	else
		IOCLR0 = (1<<22);
		

	if (cmd & 0x08)
		IOSET0 = (1<<30);
	else
		IOCLR0 = (1<<30);
		

	IOCLR1 = 0x03000000  ;	// make rs and en low
	SmallDelay() ;

	IOSET1 = 0x01000000  ;	// enable en

	SmallDelay() ;

	IOCLR1 = 0x01000000  ;	// disable en

	SmallDelay() ;
}

void LcdDat1 (unsigned char dat)
{
	if (dat & 0x01)
		IOSET0 = (1<<15);
	else
		IOCLR0 = (1<<15);
		

	if (dat & 0x02)
		IOSET0 = (1<<17);
	else
		IOCLR0 = (1<<17);
		

	if (dat & 0x04)
		IOSET0 = (1<<22);
	else
		IOCLR0 = (1<<22);
		

	if (dat & 0x08)
		IOSET0 = (1<<30);
	else
		IOCLR0 = (1<<30);
		
	IOSET1 = 0x02000000  ;	// make RS high
	SmallDelay() ;
	
	IOCLR1 = 0x01000000  ;	// disable en

	SmallDelay() ;	
		
	IOSET1 = 0x01000000  ;	// enable en

	SmallDelay() ;

	IOCLR1 = 0x01000000  ;	// disable en

	SmallDelay() ;	
}
void Delay250 (void)
{
	int k,j ;
	j =200 ;
	for(k = 0 ; k < 100 ; k ++)
	{
		j-- ;
	}
}
void DelayMs (int n)
{
	int k ;
	for(k = 0 ; k < n ; k ++)
	{
		Delay250() ;
		Delay250() ;
	}
}
void LcdCmd (unsigned char cmd)
{
	LcdCmd1(cmd >> 4) ;
	LcdCmd1(cmd) ;
	Delay250() ;
	Delay250() ;
}
void LcdDat (unsigned char dat)
{
	LcdDat1(dat >> 4) ;
	LcdDat1(dat) ;
	Delay250() ;
	Delay250() ;
}
void LcdInit (void)
{
	IODIR1 |= 0x03000000  ;
	IOCLR1 |= 0x03000000  ;

	IODIR0 |= 0x40428000  ;
	IOCLR0 |= 0x40428000  ;

	DelayMs(6) ;
	LcdCmd1(0x03) ;
	
	DelayMs(6) ;
	LcdCmd1(0x03) ;
	Delay250() ;

	LcdCmd1(0x03) ;
	Delay250() ;

	LcdCmd1(0x02) ;
	Delay250() ;


	LcdCmd(0x28) ;
	LcdCmd(0x08) ;
	LcdCmd(0x0c) ;
	LcdCmd(0x06) ;

}
void DisplayRow (int row, char *str)
{
/*
	pass pointer to 16 character string
	displayes the message on line1 or line2 of LCD, depending on whether row is 1 or 2.
*/
	int k ;

	if (row == 1)
		LcdCmd(0x80) ;
	else
		LcdCmd(0xc0) ;
	for(k = 0 ; k < 16 ; k ++)
	{
		if (str[k])
			LcdDat(str[k]) ;
		else
			break ;
	}
	while(k < 16)
	{
		LcdDat(' ') ;
		k ++ ;
	}
}

/*
	This routine is for 4x4 Matrix keyboard 

*/
void KBD_cdelay(void)
{
	int32 i32i;
	for (i32i=0; i32i<5*DEL_LOOPS; i32i++) {}
}

void KBD_Init(void)
{
	
	KBD_i32keydown=-1;
}

int32 KBD_kbhit (void)
{
	int32 i32sc , i32sc0 ;
	int32 i32ret, i32ret0 ;

	for(i32sc = 0;i32sc < 4 ; i32sc ++)
	{
		IODIR1 = KBD_u32scanport1[i32sc] | (0x03000000)	; // For RS and RW of LCD
		IOCLR1 = KBD_u32scanport1[i32sc] ;
		
		KBD_cdelay() ;
		i32ret = IOPIN1 & ALL_RET ; 

		if (KBD_i32keydown == -1)
		{
			switch(i32ret)
			{
				case	(ALL_RET & (~RET0))	:
						KBD_i32the_sc = i32sc ;
						KBD_i32keydown = (i32sc * 4) ;
						return 1 ;
				case	(ALL_RET & (~RET1))	:
						KBD_i32the_sc = i32sc ;
						KBD_i32keydown = (i32sc * 4) + 1 ;
						return 1 ;
				case	(ALL_RET & (~RET2))	:
						KBD_i32the_sc = i32sc ;
						KBD_i32keydown = (i32sc * 4) + 2 ;
						return 1 ;
				case	(ALL_RET & (~RET3))	:
						KBD_i32the_sc = i32sc ;
						KBD_i32keydown = (i32sc * 4) + 3 ;
						return 1 ;
				
			}
			
		}
		else
		{
			// i.e. key is already pressed, wait until it is released
			if (i32sc == KBD_i32the_sc)
			{
				if (i32ret == ALL_RET)
				{
					KBD_i32keydown = -1 ;	// key has been released
				}
			}
		}
	}
	return 0 ;
}

int32 KBD_rdkbd(void)
{
	while(!KBD_kbhit()) {}
	return KBD_i32keydown ;
}


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
#define  BSP_DEBUG                           1
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


/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*
*                             (c) Copyright 1992-2005, Micrium, Weston, FL
*                                          All Rights Reserved
*
*                                           Generic ARM Port
*
* File      : OS_CPU.H
* Version   : V1.61
* By        : Jean J. Labrosse
*
* For       : ARM7 or ARM9
* Mode      : ARM or Thumb
* Toolchain : IAR's EWARM V4.11a and higher
*********************************************************************************************************
*/

#ifndef  OS_CPU_H
#define  OS_CPU_H


#ifdef   OS_CPU_GLOBALS
#define  OS_CPU_EXT
#else
#define  OS_CPU_EXT  extern
#endif

/*
*********************************************************************************************************
*                                   INTERRUPT DISABLE TIME MEASUREMENT
*********************************************************************************************************
*/

#define  OS_CPU_INT_DIS_MEAS_EN    0

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

typedef unsigned int   OS_STK;                   /* Each stack entry is 32-bit wide                    */
typedef unsigned int   OS_CPU_SR;                /* Define size of CPU status register (PSR = 32 bits) */

/*
*********************************************************************************************************
*                                                ARM
*
* Method #1:  Disable/Enable interrupts using simple instructions.  After critical section, interrupts
*             will be enabled even if they were disabled before entering the critical section.
*             NOT IMPLEMENTED
*
* Method #2:  Disable/Enable interrupts by preserving the state of interrupts.  In other words, if
*             interrupts were disabled before entering the critical section, they will be disabled when
*             leaving the critical section.
*             NOT IMPLEMENTED
*
* Method #3:  Disable/Enable interrupts by preserving the state of interrupts.  Generally speaking you
*             would store the state of the interrupt disable flag in the local variable 'cpu_sr' and then
*             disable interrupts.  'cpu_sr' is allocated in all of uC/OS-II's functions that need to
*             disable interrupts.  You would restore the interrupt disable state by copying back 'cpu_sr'
*             into the CPU's status register.
*********************************************************************************************************
*/

#define  OS_CRITICAL_METHOD    3


#if      OS_CRITICAL_METHOD == 3

#if      OS_CPU_INT_DIS_MEAS_EN > 0

#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();  \
                               OS_CPU_IntDisMeasStart();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_IntDisMeasStop();   \
                               OS_CPU_SR_Restore(cpu_sr);}

#else

#define  OS_ENTER_CRITICAL()  {cpu_sr = OS_CPU_SR_Save();}
#define  OS_EXIT_CRITICAL()   {OS_CPU_SR_Restore(cpu_sr);}

#endif

#endif

/*
*********************************************************************************************************
*                                         ARM Miscellaneous
*********************************************************************************************************
*/

#define  OS_STK_GROWTH        1                   /* Stack grows from HIGH to LOW memory on ARM        */

#define  OS_TASK_SW()         OSCtxSw()

/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                  /* Variables used to measure interrupt disable time  */
#if OS_CPU_INT_DIS_MEAS_EN > 0
OS_CPU_EXT  INT16U  OS_CPU_IntDisMeasNestingCtr;
OS_CPU_EXT  INT16U  OS_CPU_IntDisMeasCntsEnter;
OS_CPU_EXT  INT16U  OS_CPU_IntDisMeasCntsExit;
OS_CPU_EXT  INT16U  OS_CPU_IntDisMeasCntsMax;
OS_CPU_EXT  INT16U  OS_CPU_IntDisMeasCntsDelta;
OS_CPU_EXT  INT16U  OS_CPU_IntDisMeasCntsOvrhd;
#endif

/*
*********************************************************************************************************
*                                              PROTOTYPES
*********************************************************************************************************
*/
#define __arm 
#if OS_CRITICAL_METHOD == 3
__arm  OS_CPU_SR  OS_CPU_SR_Save(void);           /* See OS_CPU_A.S                                    */
__arm  void       OS_CPU_SR_Restore(OS_CPU_SR cpu_sr);
#endif
__arm  void       OS_CPU_IRQ_ISR(void);
__arm  void       OS_CPU_FIQ_ISR(void);
__arm  void       OSCtxSw(void);
__arm  void       OSIntCtxSw(void);
//__arm  void       OSStartHighRdy(void);
__arm  void       OSStartHighRdy(void);


       void       OS_CPU_IRQ_ISR_Handler(void);   /* See BSP.C                                         */
       void       OS_CPU_FIQ_ISR_Handler(void);
#if OS_CPU_INT_DIS_MEAS_EN > 0
       void       OS_CPU_IntDisMeasInit(void);
       void       OS_CPU_IntDisMeasStart(void);
       void       OS_CPU_IntDisMeasStop(void);
       INT16U     OS_CPU_IntDisMeasTmrRd(void);
#endif

#endif

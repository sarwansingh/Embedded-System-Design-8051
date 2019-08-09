/*
*********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                             (c) Copyright 1998-2005, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
*                                            
*
* File : APP.C
*********************************************************************************************************
*/

#include "includes.h"
/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary ARM and C initialization.
* Arguments   : none
*********************************************************************************************************
*/
OS_STK Task1Stk[1024];
OS_STK Task2Stk[1024];
OS_STK Task3Stk[1024];

int  main (void)
{
    BSP_Init();                           /* Initialize BSP functions                                */

    OSInit();                              /* Initialize "uC/OS-II, The Real-Time Kernel"              */
	
	
	OSTaskCreate(App_Task1,(void *)0,&Task1Stk[1023],6);
    OSTaskCreate(App_Task2,(void *)0,&Task2Stk[1023],7);
	OSTaskCreate(App_Task3,(void *)0,&Task3Stk[1023],8);

    OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)       */
    while(1);

}

unsigned int i8ch ;
char szTemp[16] ;

void  App_Task1 (void *pdata);
void  App_Task1 (void *pdata)
{
    (void)pdata;
    
	IO0DIR |= 0x00004000 ;

    while(1)
    {
    	IO0CLR = (1<<14) ;
    	OSTimeDlyHMSM(0,0,0,500);
		IO0SET = (1<<14) ;
        OSTimeDlyHMSM(0,0,0,500);    
    }
}

void  App_Task2 (void *pdata);
void  App_Task2 (void *pdata)
{
    (void)pdata;

    while(1)
    {
        
        printf("N");
        OSTimeDlyHMSM(0,0,0,600);

    }
}

void  App_Task3 (void *pdata);
void  App_Task3 (void *pdata)
{
    (void)pdata;
    
    DisplayRow(1,"Keypad Test ");
	DisplayRow(2,"Press Any Key  ");


    while(1)
    {
    
        i8ch = KBD_rdkbd() ;

        sprintf(szTemp,"KeyCode = %X",i8ch);
		DisplayRow(2,szTemp) ;					

        OSTimeDlyHMSM(0,0,0,500);

    }
}

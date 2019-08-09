
#include "includes.h"

/////////////////lcd 
void SmallDelay (void){	int i;	for(i=0;i<100;i++);	}

void LcdCmd1 (unsigned char cmd)
{	// send command on on data lines (D4 to D7)
	if (cmd & 0x01)	IO0SET = (1<<15);
	else		IO0CLR = (1<<15);
		
	if (cmd & 0x02)	IO0SET = (1<<17);
	else		IO0CLR = (1<<17);
		
	if (cmd & 0x04)	IO0SET = (1<<22);
	else		IO0CLR = (1<<22);
		
	if (cmd & 0x08)	IO0SET = (1<<30);
	else		IO0CLR = (1<<30);
		
	IO1CLR = 0x03000000  ;	// CLEAR(0) RS and EN 
	SmallDelay() ;

	IO1SET = 0x01000000  ;	// SET(1) EN 
	SmallDelay() ;

	IO1CLR = 0x01000000  ;	// CLEAR(0) EN 
	SmallDelay() ;
}

void LcdDat1 (unsigned char dat)
{
	// send data on on data lines (D4 to D7)
	if (dat & 0x01)	IO0SET = (1<<15);
	else		IO0CLR = (1<<15);
		
	if (dat & 0x02)	IO0SET = (1<<17);
	else		IO0CLR = (1<<17);

	if (dat & 0x04)	IO0SET = (1<<22);
	else		IO0CLR = (1<<22);
		
	if (dat & 0x08)	IO0SET = (1<<30);
	else		IO0CLR = (1<<30);
		
	
	IO1SET = 0x02000000  ;	// SET(1) RS 
	SmallDelay() ;
	
	IO1CLR = 0x01000000  ;	// CLEAR(0) EN 
	SmallDelay() ;	
		
	IO1SET = 0x01000000  ;	// SET(1) EN 
	SmallDelay() ;

	IO1CLR = 0x01000000  ;	// CLEAR(0) EN 

	SmallDelay() ;	
}
void Delay250 (void)
{	int k,j ;	j =200 ;
	for(k = 0 ; k < 100 ; k ++)	{j-- ;}
}
void DelayMs (int n)
{ 	int k,i ;
	for(k = 0 ; k < n ; k ++)
	{	 for(i=0;i<1000;i++ );	}
}
void LcdCmd (unsigned char cmd)
{
	LcdCmd1(cmd >> 4) ;
	LcdCmd1(cmd) ;
	Delay250() ;	Delay250() ;
}
void LcdDat (unsigned char dat)
{
	LcdDat1(dat >> 4) ;
	LcdDat1(dat) ;
	Delay250() ;	Delay250() ;
}
void LcdInit (void)
{
	IO1DIR = 0x03000000  ;		// Configure P1.24(EN) and P1.25(RS) as Output
	IO1CLR = 0x03000000  ;		// CLEAR(0) P1.24(EN) and P1.25(RS)

	IO0DIR = 0x40428000  ;		// Configure P0.15(D4), P0.17(D5), P0.22(D6) and P0.30(D7) as Output
	IO0CLR = 0x40428000  ;		// CLEAR(0) P0.15(D4), P0.17(D5), P0.22(D6) and P0.30(D7)

	DelayMs(6) ;	LcdCmd1(0x03) ;
	DelayMs(6) ;	LcdCmd1(0x03) ;
	Delay250() ;	LcdCmd1(0x03) ;
	Delay250() ;	LcdCmd1(0x02) ;
	Delay250() ;	LcdCmd(0x28) ;
					LcdCmd(0x08) ;
					LcdCmd(0x0c) ;
					LcdCmd(0x06) ;

}

void  App_Task1 (void *p_arg);
void  App_Task2 (void *p_arg);
void  App_Task3 (void *p_arg);

/////////////////////
OS_STK Task1Stk[1024];
OS_STK Task2Stk[1024];
OS_STK Task3Stk[1024];

int  main (void)
{
    BSP_Init();					/* Initialize BSP functions                                */

	LcdInit();	
	IO0DIR |= 0x003C3C00  ;		// P0.10 to P0.13 and P0.18 to P0.21 are set as Output
	IO0SET = 0x003C3C00  ;
	IO0DIR |= (1<<25); // buzzer (0.25)
    
    OSInit();					/* Initialize "uC/OS-II, The Real-Time Kernel"              */
	
 
	OSTaskCreate(App_Task1,(void *)0,&Task1Stk[1023],6);
    OSTaskCreate(App_Task2,(void *)0,&Task2Stk[1023],7);
	OSTaskCreate(App_Task3,(void *)0,&Task3Stk[1023],8);
    
    OSStart();                             /* Start multitasking (i.e. give control to uC/OS-II)       */
    while(1);

}

void  App_Task1 (void *p_arg)
{
    (void)p_arg;
    
    while(1)
    {   /* repeate forever */
    	IO0CLR = 0x003C3C00  ;
    	OSTimeDlyHMSM(0,0,0, 500);
		IO0SET = 0x003C3C00  ;
        OSTimeDlyHMSM(0,0,0, 500);
    }
}
void  App_Task2 (void *p_arg)
{
    (void)p_arg;
    	char row1[] = "NIELIT Chandigarh" ;   int i;
    while(1)
    {	  LcdCmd(0x01) ; // clear screen
     	for ( i=0; row1[i] !='\0' ;i++ )
		 { 
		 	LcdDat( row1[i]); 
    		OSTimeDlyHMSM(0,0,4 ,0);
    	 
		 }
         
    }
}
void  App_Task3 (void *p_arg)
{
    (void)p_arg;
    
    while(1)
    {
    	IO0CLR = 1<<25 ;
    	OSTimeDlyHMSM(0,0,0,250);
		IO0SET = 1<<25;
        OSTimeDlyHMSM(0,0,0,250);
    }

}
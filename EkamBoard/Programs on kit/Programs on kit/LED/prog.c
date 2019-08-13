#include<reg51.h>
sbit BUZZER=P3^7;
void mydelay()
{	  int i,j;
 	for(j=0;j<2;j++){
			for (i=0;i<30000;i++);  
		}
}
void main()
{		BUZZER=0;					 
 	while(1)
	{
	 	P2 = 0x00  ;   mydelay();
	   	P2 = 0xff ;	mydelay();

		P1 = 0xaa  ;   mydelay();
	   	P1 = 0x55 ;	mydelay();
		
		P3 = 0xcc  ;   mydelay();
	   	P3 = 0x33 ;	mydelay();
	
		P0 = 0x66  ;   mydelay();
	   	P0 = 0x99 ;	mydelay();
	}
}
/*
  	P1 = 0xaa; 
		for (i=0;i<30000;i++);
		P1 = 0x55;
		for (i=0;i<30000;i++);
*/
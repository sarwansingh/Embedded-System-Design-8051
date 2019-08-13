#include<reg51.h>
sbit BUZZER=P3^7;
sbit But1 = P2^7;
sbit But2 = P2^6;
sbit But3 = P2^5;
sbit But4 = P2^4;
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
		if (But1 ==0 ) 
		{ 
			P1 = 0xaa  ;   mydelay();
	   		P1 = 0x55 ;	mydelay();
		}
		if (But2 ==0 ) 
		{ 
	 		P1 = 0x00  ;   mydelay();
	   		P1 = 0xff ;	mydelay();
		}
		if (But3 ==0 ) 
		{ 
			P1 = 0xcc  ;   mydelay();
		   	P1 = 0x33 ;	mydelay();
		}
	   	if (But4 ==0 ) 
		{ 
			P1 = 0x66  ;   mydelay();
	   		P1 = 0x99 ;	mydelay();
		}
	}
}

#include<reg51.h>
sbit but1= P3^0;
sbit LED1= P2^0;
void mydelay(){
	int i =0;
	for (i=0;i<30000;i++);
}
void main()
{	 P2=0x00; 
 	while (1){
	  if (but1 ==0 )	{	 //button is pressed
   		 LED1 =1; mydelay();
	  }else {		   //button not pressed
	  	 LED1 =0; mydelay();
	  }
		
	}
}
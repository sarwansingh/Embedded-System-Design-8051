#include<reg51.h>
sbit but1= P3^0;
sbit LED1= P2^0;
void mydelay(){
	int i =0;
	for (i=0;i<30000;i++);
}
void main()
{	int butStatus=0; 
	P2=0x00; 
 	while (1){
	  if (but1 ==0 )	{	 //button is pressed
	  	 // toggle the value of a 
		 if (butStatus==1) butStatus=0; 
		 else butStatus=1;

		 while (but1 ==0 ); // wait here till button is released 

   		 if (butStatus==1){LED1 =1; mydelay(); }
		 else {LED1 =0; mydelay(); }
		 
	  }

	}
}
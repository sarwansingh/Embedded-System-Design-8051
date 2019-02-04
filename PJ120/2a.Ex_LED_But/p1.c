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
		 while (but1 ==0 ); // wait here till button is released 
 		  butStatus++;	
		  if (butStatus >3) butStatus =1;	 
	  }
	  if (butStatus ==1){
	  	//pattern1 
		  P2=0xff; mydelay(); 
		  P2 =0x00; mydelay(); 	
	  }
	   if (butStatus ==2){
		  //pattern2
		  P2 =0xaa; mydelay(); 
		  P2 =0x55; mydelay(); 
		}
	  if (butStatus ==3){
		  //pattern 3
		  P2 = 0x00; 
		}
	  

	}
}
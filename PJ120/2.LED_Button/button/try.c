#include<reg51.h>
sbit but1= P3^0;
sbit LED1= P2^0;
void mydelay(){
	int i =0;
	for (i=0;i<30000;i++);
}
void main()
{ 
 	while (1){
	  if (but1 ==0 )	{	 //button is pressed
		 while (but1 ==0 ); // wait here till button is released 
 		 // do some kaam   
	  }
	 } 
}
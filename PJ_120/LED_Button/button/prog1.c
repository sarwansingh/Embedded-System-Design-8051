#include<reg51.h>
void mydelay(){
	int i =0;
	for (i=0;i<30000;i++);
}
void main()
{
 	while (1){
	  if (P3==0xfe)	{	 //1111 1110
   		P2= 0xff; mydelay();
	  }else {		   //1111 1111  ff
	  	P2= 0x00; mydelay();
	  }
		
	}
}
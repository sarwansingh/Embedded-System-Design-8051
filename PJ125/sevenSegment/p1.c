#include <reg51.h>
sbit X1=P3^2;
sbit X2=P3^3;
sbit X3=P3^4;
sbit X4=P3^5;
			   
void mydelay(int t){
   int x=0,y=0;
   for (x=0 ; x<t; x++)
   {  for (y=0 ; y<200; y++){ 		}		
   }
}

void main(){
	X1=X2=X3=X4=0; 
  while(1){
  	 X1=1; // switch on first seven segment 
 	  P0=0x86;   // make segment on for 1	  //1000 0110	   
	  mydelay(300);
	  P0=0xdb; // make segment on for 2	  //1101 1011
	  mydelay(300);
  }
}

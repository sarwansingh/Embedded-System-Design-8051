  
#include <reg51.h>
sbit buzzer = P0^4;
void mydelay(int t){
   int x=0,y=0;
   for (x=0 ; x<t; x++)
   {  for (y=0 ; y<20000; y++){ 		}		
   }
}
void main(){
	  while(1){
	   	buzzer=1;
		mydelay(10);
		buzzer=0;
		mydelay(10);
	  }
}
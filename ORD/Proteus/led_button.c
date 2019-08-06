#include <reg51.h>
#include<functions.c>

void mydelay() ; // function declaration
sbit  BUTTON1= P2^0;
int status=0;

void main(){
	while(1){
		if (BUTTON1 == 0 ) {
			 if (status ==1) status=0;
			 else status=1;
			 while (BUTTON1 == 0 ); 
		}  
		if (status ==1) {
		 	P1 = 0xaa; mydelay();
			P1 = 0x55; mydelay();
		}
		else  P1 = 0x00; 

	}
}

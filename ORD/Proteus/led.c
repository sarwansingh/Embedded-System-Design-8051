#include <reg51.h>
void mydelay() ; // function declaration
void main(){
	while(1){
	 P1 = 0xaa; mydelay();
	 P1 = 0x55; mydelay();
	}
}
void mydelay(){
	 int i ;
	 for (i=0;i<32000;i++) ; 
}
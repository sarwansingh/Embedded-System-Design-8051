#include <reg51.h>
void mydelay()
{	unsigned int i; 
	for(i=0;i<60000; i++); 
}
void main()
{
	while(1){
		P2 = 0xaa;   mydelay();// 1010 1010
		P2 = 0x55; 	 mydelay();// 0101 0101
//		P2 = 170;   mydelay();// 1010 1010
//		P2 = 85; 	 mydelay();// 0101 0101
	}
}
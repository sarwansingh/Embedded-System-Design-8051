#include<reg51.h>
void mydelay()
{	unsigned int i ;
	for (i=0;i<40000;i++);
}
void main()
{
 	while(1){		 		   //xgfe dcba 
	 	P2= 0xc0; mydelay() ; ///1100 0000  -zero
	 	P2= 0xf9; mydelay();  ///1111 1001  -one
	 	P2= 0xa4; mydelay();  ///1010 0100  -two

	}
}
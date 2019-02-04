#include<reg51.h>
void mydelay()
{	unsigned int i ;
	for (i=0;i<40000;i++);
}
void display(int i )
{						 //xgfe dcba 
 	if(i==0)P2= 0xc0;   ///1100 0000  -zero
	if(i==1) P2= 0xf9;  ///1111 1001  -one
	if(i==2) P2= 0xa4;  ///1010 0100  -two	
	mydelay()  ;
}	
void main()
{		int cnt; 
 	while(1){		 		   
		for (cnt=0; cnt<=2; cnt++) display(cnt); 
	}				   
}
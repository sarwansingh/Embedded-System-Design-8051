#include<reg51.h>
sbit BUT0 = P2^0; 
void mydelay( int n)
{	  int i,j;
 	for(j=0;j<n;j++){
			for (i=0;i<30000;i++){  }    // 1msec 
		}
}

void main()
{					 
 	while(1)					// 1 off , 0 led on
	{	
	//	if (P2 == 0xfe )		//1111 1110
		if( BUT0 == 1)
		{ 
			P1 = 0x00; mydelay(4);
			P1 = 0xff; mydelay(4);
		}
	}
}
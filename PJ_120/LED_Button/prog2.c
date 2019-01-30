#include <reg51.h>
// freq/delay of led blink should change  after 5 times it blink 
void mydelay(int d)
{	unsigned int i,j; 
	for ( j=0 ; j<d ;j++){
		for(i=0;i<20000; i++); 
	}
}
void main()
{	int k=0,h,dd=8;
	while(1){
		for (h=0;h<3;h++){
	   	  for (k=0 ; k<5; k++){
			P2 = 0xff;   mydelay(dd);// 1010 1010
			P2 = 0x00; 	 mydelay(dd);// 0101 0101
		  }
		  dd=dd/2;
		}		 
	}
}
/*
	for (k=0 ; k<5; k++){
			P2 = 0xff;   mydelay(9);// 1010 1010
			P2 = 0x00; 	 mydelay(9);// 0101 0101
		}
		for (k=0 ; k<5; k++){
			P2 = 0xff;   mydelay(5);// 1010 1010
			P2 = 0x00; 	 mydelay(5);// 0101 0101
		}
		for (k=0 ; k<5; k++){
			P2 = 0xff;   mydelay(1);// 1010 1010
			P2 = 0x00; 	 mydelay(1);// 0101 0101
		}*/
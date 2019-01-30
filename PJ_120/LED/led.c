#include <reg51.h>
//define function 
delay( int x  ) 
{	int i;
	for ( ; x>0 ; x--){
		for (i=0; i<255; i++){   } 	
	}
}
void main()
{
	while(1){
//	  	P2 = 0x00;	   // 0000 0000
//		delay(400); 
//		P2 = 0xff;	   // 1111 1111
 		
		 P2 = 0x01;	 //0000 0001
		delay(400); 
		P2 = 0x02;	 //0000 0010
		delay(400);
	}
}
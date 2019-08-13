#include<reg51.h>
void mydelay()
{
 	int i,j; 
	for (i=0 ; i<3;i++)
		for(j=0;j<30000;j++) ;
}
void display7( int n)
{

		if (n==0){ P2= 0xc0;	} 	//0
	 	if (n==1){ P2= 0xf9;	}	 //1
		if (n==2){P2= 0xa4; 	}		 //2
		
	 	if (n==3){P2= 0xb0; }		 //3
		if (n==4){P2= 0x99; }		 //4
	 	if (n==5){P2= 0x92; }		 //5
		if (n==6){P2= 0x82; }		 //6
	 	if (n==7){P2= 0xf8; }		 //7
		if (n==8){P2= 0x80; }		 //8
	 	if (n==9){P2= 0x90; }		 //9
 		  mydelay();
//		P2= 0x88; mydelay();	//A
//	 	P2= 0x83; mydelay();	//b
//		P2= 0xc6; mydelay();	//C
//	 	P2= 0xa1; mydelay();	//d
//		P2= 0x86; mydelay();	//E
//	 	P2= 0x8e; mydelay();	//F

}
void main()
{		  int i;
 	while(1)
	{
		for(i=0;i<10;i++)	 display7(i); 
		for(i=10;i>0;i--)	 display7(i); 
		 
	}
}
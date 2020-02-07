#include <reg51.h>
sbit X1=P3^2;
sbit X2=P3^3;
sbit X3=P3^4;
sbit X4=P3^5;
char arr[10]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xe7};
			   
void mydelay(int t){
   int x=0,y=0;
   for (x=0 ; x<t; x++)
   {  for (y=0 ; y<200; y++){ 		}		
   }
}

void main(){
	int j; 
	X1=X2=X3=X4=0; 
  while(1){
  	 // switch on first seven segment 
    int cnt =12; int r,q; 
	for (cnt=0 ; cnt<99; cnt++){ 
	   	for(j=0;j<=100;j++)
		{				 r = cnt %10; q = cnt /10;
			X2=0;X1=1;  P0=arr[q];mydelay(1);	   
		  	X2=1;X1=0;  P0=arr[r]; 	mydelay(1);	   
		}
	}

  }
}

/*	step 1 
  	  P0=0x86; // make segment on for 1	  //1000 0110
	  mydelay(10);
	  P0=0xdb; // make segment on for 2	  //1101 1011
	  mydelay(10);

//step no 2

		X4=0;X3=0; X2=0;X1=1;	P0=0x86;mydelay(1);
	  	X4=0;X3=0;X2=1;X1=0;  P0=0xdb; 	mydelay(1);
		X4=0;X3=1;X2=0;X1=0;  P0=0xcf; 	mydelay(1);
		X4=1;X3=0;X2=0;X1=0;  P0=0xe6; 	mydelay(1);

//step 3
X4=0;X3=0; X2=0;X1=1;	P0=0x86;mydelay(1);	   //1
	  	X4=0;X3=0;X2=1;X1=0;  P0=0xdb; 	mydelay(1);	   //4
		X4=0;X3=1;X2=0;X1=0;  P0=0xcf; 	mydelay(1);	   //3
		X4=1;X3=0;X2=0;X1=0;  P0=0xe6; 	mydelay(1);	   //2
*/
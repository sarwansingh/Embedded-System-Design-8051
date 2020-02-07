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
	int j,a,b,c,i; 
	X1=X2=X3=X4=0; 
  while(1){
  	  
	for (j=0;j<199 ; j++){

	 a= j/100 ;   b= (j/10) %10; c= j%10;
	   
	   for(i=0;i<50;i++){// this loop will display the data statically
		X2=1; X3=0; X4=0;  P0= arr[a]; mydelay(3);
		X2=0; X3=1; X4=0;  P0= arr[b]; mydelay(3);
		X2=0; X3=0; X4=1;  P0= arr[c]; mydelay(3);
	   }
	}
  }
}


/*
j=345
a= j/100 ; 
b= (j/10) %10
c= j%10;

// display 12 on two seven segments 
	 // 1 on X3, 2 on X4
	X3=1; X4=0;  P0= arr[1]; mydelay(3);
	X3=0; X4=1;  P0= arr[2]; mydelay(3);
*/

 
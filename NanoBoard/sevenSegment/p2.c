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
  	 X1=1; // switch on first seven segment
	 for (j=0 ;j<=9;j++){
	 	P0= arr[j]; mydelay(300);
	 } 
//	 P0= arr[0]; mydelay(300);
//	 P0= arr[1]; mydelay(300);
//	 P0= arr[2]; mydelay(300);
  }
}
 
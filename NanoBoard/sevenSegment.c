#include <reg51.h>
sbit X1=P3^2;
sbit X2=P3^3;
sbit X3=P3^4;
sbit X4=P3^5;
char arr[10]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xe7},j,j1,j2;
			   
void mydelay(int t){
   int x=0,y=0;
   for (x=0 ; x<t; x++)
   {  for (y=0 ; y<20000; y++){ 		}		
   }
}

void main(){
	X2=X3=X4=0;
  while(1){
   	for(j=0;j<=9;j++)
	{
			X1=1;	P0=arr[j];
			mydelay(10);
	}
  }
}
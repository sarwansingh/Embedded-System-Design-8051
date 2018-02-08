#include<reg51.h>
void main()
{  	int i;
	while(1){
	   P2=0xaa;   //1010 1010
	   for(i=0;i<10700 ;i++){} // generate delay
	   P2=0x55;	  //0101 0101 
	   for(i=0;i<107 ;i++){}	// generate delay
	    P2=0x53;	  //0101 0101 
	   for(i=0;i<10700 ;i++){}	// generate delay
	    P2=0x55;	  //0101 0101 
	   for(i=0;i<107 ;i++){}	// generate delay
	    P2=0x55;	  //0101 0101 
	   for(i=0;i<10700 ;i++){}	// generate delay
   }
}
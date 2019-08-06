/*
Port2 has eight LEDs
P1^0 has button .
step1 : first time button is pressed LED at P2^0 should be on 
second time button is pressed LED at P2^1 should be on 
....
eighth  time button is pressed LED at P2^7 should be on
ninth time all LEDs off 
tenth time button is pressed go to Step 1 
*/
#include<reg51.h>
// initialize single bit variable pointing to P1^0
sbit but1 =P1^0; 
void main()
{  	 	int i=0; char j; 
		P2=0xff;  j=P2;  	
	while(1){
		if (but1 ==0){ // button pressed
			// Logic of the program 
			if (j==0) {	P2=0xff; j=P2;}
		 	j = j<<1;				 
			P2 = j;
			// Logic end of the program 
			while (but1 ==0); // wait till user release button 
		}
	}
}
/*		skeleton of button interfacing 
sbit but1 =P1^0;  // initialise the Port pin with but1 variable 

	if (but1 ==0){ // button pressed
		// Logic of the program 
		while (but1 ==0); // wait till user release button 
	}
*/
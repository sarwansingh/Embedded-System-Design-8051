#include<reg51.h>
// initialize single bit variable pointing to P1^0
sbit but1 =P1^0; 
void my_delay(int delay){   
	int i,j; 
	for ( j=0 ; j< delay ; j++){
		for(i=0 ; i<30000;i++) ; 	// 1 sec 
	}
}
/*
by default P1 is 1111 1111
if Button is connected to P1.0 and other side of button is on ground
then if button is pressed P1 will become 1111 1110	(0xfe or 254)
*/
void main()
{  	 int button_status =0 ; //0-off , 1-on
	while(1){
		 // check the status of the button
		 if ( but1 == 0){
		 	if ( button_status==1) 
				 button_status=0;
			else button_status =1;
			while( but1 == 0); // wait till user releases the button 
		 }
		
		 if (button_status == 1) 	P2= 0x00;  	// all on 		
		 else						P2= 0xff; //all off  
	}
}	 	//my_delay(3);

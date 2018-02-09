#include<reg51.h>
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
		 if ( P1== 0xfe){
		 	if ( button_status==1) 
				 button_status=0;
			else button_status =1;
			while( P1== 0xfe); // wait till user releases the button 
		 }
		
		 if (button_status == 1) 	P2= 0x00;  	// all on 		
		 else						P2= 0xff; //all off  
	}
}	 	//my_delay(3);
/* v1
	while(1){
		if ( P1== 0xfe){
			P2= 0x0f;  my_delay(3);
			P2= 0xf0;  my_delay(3);
		}
	}

	version 2
	while(1){
		 if ( P1== 0xfe)
			P2= 0x0f;  			
		 else
			P2= 0xf0;  
		my_delay(3);
	}

	version3
	 	while(1){
		 if ( P1== 0xfe)
			P2= 0x00;  	// all on 		
		 else
			P2= 0xff; //all off  
		my_delay(3);
	}
	version4
	int button_status =0 ; //0-off , 1-on
	while(1){
		 // check the status of the button
		 if ( P1== 0xfe) button_status=1;
		
		 if (button_status == 1) 	P2= 0x00;  	// all on 		
		 else						P2= 0xff; //all off  
	}
	version 5
	 // check the status of the button
		 if ( P1== 0xfe){
		 	if ( button_status==1) 
				 button_status=0;
			else button_status =1;
		 }
		
		 if (button_status == 1) 	P2= 0x00;  	// all on 		
		 else						P2= 0xff; //all off  
*/
#include<reg51.h>
sbit R1 = P2^4;
sbit R2 = P2^5;
sbit R3 = P2^6;
sbit R4 = P2^7;

sbit C1=  P2^3;
sbit C2=  P2^2;
sbit C3=  P2^1;
sbit C4=  P2^0;
 
#define LCDDATA P0
sbit RS = P1^0;
sbit RW = P1^1;
sbit E = P1^2;	
sbit BUZZER = P3^7;

void mydelay()
{		   int i;
 	for(i=0;i<30000;i++);
}
void lcd_data(char x)
{	RS = 1;	  	E=1;
	LCDDATA =x;	mydelay();
	//BUZZER = 0;
	E=0; 	
}
void lcd_cmd(char x)
{	RS =0 ;	  	E=1;
	LCDDATA =x;	mydelay();
	E=0; 	
}
void lcd_init()
{	  	 
		lcd_cmd(0x38);
		lcd_cmd(0x01);
		lcd_cmd(0x0e);
}
void main()
{	BUZZER =0; RW=0;	 
	lcd_init(); 	
	lcd_data('A');
	R1=R2=R3=R4=1;  C1=C2=C3=C4=1; 
  	while(1){
 		 R1=0; 
		 if (C1==0){ lcd_data('4') ; while(C1==0);} 
		 if (C2==0){ lcd_data('3') ; while(C2==0);} 
		 if (C3==0){ lcd_data('2') ; while(C3==0);} 
		 if (C4==0){ 
		 	lcd_data('1') ; while(C3==0);
//			lcd_cmd(0x10) ; 
//			lcd_data(' ') ;
//		  	lcd_cmd(0x10) ; 
			while(C4==0);
		} 
		 R1=1; R2=0; 
		if (C1==0){ lcd_data('8') ; while(C1==0);} 
		if (C2==0){ lcd_data('7') ; while(C2==0);} 
		if (C3==0){ lcd_data('6') ; while(C3==0);} 
		if (C4==0){ lcd_data('5') ;	while(C4==0);} 

		R2=1; R3=0; 
		if (C1==0){ lcd_data('c') ; while(C1==0);} 
		if (C2==0){ lcd_data('b') ; while(C2==0);} 
		if (C3==0){ lcd_data('a') ; while(C3==0);} 
		if (C4==0){ lcd_data('9') ;	while(C4==0);} 

		 R3=1; R4=0; 
		if (C1==0){ lcd_data('g') ; while(C1==0);} 
		if (C2==0){ lcd_data('f') ; while(C2==0);} 
		if (C3==0){ lcd_data('e') ; while(C3==0);} 
		if (C4==0){ lcd_data('d') ;	while(C4==0);} 

		R1=R2=R3=R4=1; 
	}
}
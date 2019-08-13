#include<reg51.h>
sbit R1 = P2^0;
sbit R2 = P2^1;
sbit R3 = P2^2;
sbit R4 = P2^3;

sbit C1=  P2^4;
sbit C2=  P2^5;
sbit C3=  P2^6;
sbit C4=  P2^7;

sbit RS = P3^0;
sbit E = P3^1;
void mydelay()
{		   int i;
 	for(i=0;i<30000;i++);
}
void lcd_data(char x)
{	RS = 1;	  	E=1;
	P1 =x;	mydelay();
	E=0; 	
}
void lcd_cmd(char x)
{	RS =0 ;	  	E=1;
	P1 =x;	mydelay();
	E=0; 	
}
void lcd_init()
{	  	 
		lcd_cmd(0x38);
		lcd_cmd(0x01);
		lcd_cmd(0x0e);
}
void main()
{
	P2 =0xff;  lcd_init() ;
 	while(1)
	{
	 	R1=0;
		if (C1==0) { lcd_data('1' ); while(C1==0); } 
		if (C2==0) { lcd_data('2' );while(C2==0);} 
		if (C3==0) 	{lcd_data('3' ); while(C3==0);} 
		if (C4==0) { lcd_data('4' );while(C4==0);} 
		R1=1;R2=0;
		if (C1==0) { lcd_data('5' ); while(C1==0); } 
		if (C2==0) { lcd_data('6' );while(C2==0);} 
		if (C3==0) 	{lcd_data('7' ); while(C3==0);} 
		if (C4==0) { lcd_data('8' );while(C4==0);} 
		R2=1;
	}

}
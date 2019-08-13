#include<reg51.h>
#define LCDDATA P0
sbit RS = P1^0;
sbit RW = P1^1;
sbit E  = P1^2;	
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
char name[] = "India is great country" ; 
void main()
{	  	int i;
		RW = 0; 
		BUZZER=1; // on 
		mydelay();mydelay();mydelay();
		BUZZER = 0; 	//off
		lcd_cmd(0x38);
		lcd_cmd(0x01);
		lcd_cmd(0x0e);
	while(1)
	{
		for (i=0;name[i] != '\0' ;i++ ){
	 		lcd_data(name[i]); 
			if (i==15) 	 lcd_cmd(0xc0);	   // move to next line 
		}  	
		lcd_cmd(0x01);
		//lcd_data(name[1]); mydelay();
	}

}
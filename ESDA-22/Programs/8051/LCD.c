#include <reg51.h>
#define LCDDATA P2
sbit RS	=	P3^6;
sbit E	=	P3^7;

void mydelay(int t){
   int x=0,y=0;
   for (x=0 ; x<t; x++)
   {  for (y=0 ; y<20000; y++){ 		}		
   }
}

void lcd_cmd(char d)
{
	RS=0;
	E=1;	LCDDATA= d;	mydelay(1); E=0;
}

void lcd_data(char d)
{
	RS=1;
	E=1;	LCDDATA= d; mydelay(1); E=0;
}
char msg[] = "India is Great"; int i;
void main(){
	  while(1){
			//initialize the LCD 
				lcd_cmd(0x38);
				lcd_cmd(0x0E);
				lcd_cmd(0x01);	    
				lcd_data('N');				lcd_data('I');
				lcd_data('E');				lcd_data('L');
				lcd_data('I');				lcd_data('T');
				lcd_data(' ');lcd_data('D');lcd_data('R');
				lcd_data('D');lcd_data('O');
			  lcd_cmd(0xc0);	
				for (i=0 ; msg[i] != '\0' ; i++)
						lcd_data(msg[i]);
			
					mydelay(100);
	  }
}
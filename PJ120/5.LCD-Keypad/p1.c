#include<reg51.H>
#define LCDPORT P2
sbit RS=P3^0;
sbit E =P3^1;

void my_delay(int i)
{int j; 
 	for (;i>0;i--)
		for (j=0; j<30000;j++){}
}
void LCD_Cmd ( int  i ) {
      RS = 0;	LCDPORT = i ; // RS=0 for command 
      E  = 1 ;   	my_delay(1);      E  = 0 ;
}
void LCD_Data ( int  i ) {
      RS = 1;	LCDPORT = i ;  // RS=1 for data
      E  = 1 ;   	my_delay(1);     E  = 0 ;
}
void main()
{	// initialize the LCD 
	LCD_Cmd (0x38); LCD_Cmd (0x01);  LCD_Cmd (0x0e);
	while(1){	 
		LCD_Data ( 'I');LCD_Data ( 'n');LCD_Data ( 'd');
	}

}
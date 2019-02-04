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
      RS = 0;	// RS=0 for command 
      E  = 1 ;   LCDPORT = i ; 	my_delay(1);      E  = 0 ;
}
void LCD_Data ( int  i ) {
      RS = 1;	 // RS=1 for data
      E  = 1 ;   LCDPORT = i ; 	my_delay(1);     E  = 0 ;
}
void main()
{	 LCD_Cmd(0x38);	 my_delay(1);
		LCD_Cmd(0x38);	 my_delay(1);
		LCD_Cmd(0x01);	 my_delay(1);
		LCD_Cmd(0x0e);	 my_delay(1);
		while(1){
			LCD_Data('P');	 my_delay(1); 
			LCD_Data('J');	 my_delay(1); 
		}
}
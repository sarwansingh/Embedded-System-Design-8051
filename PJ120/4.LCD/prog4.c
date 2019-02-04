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
void LCD_init(){
 	LCD_Cmd(0x38);	 my_delay(1);
	LCD_Cmd(0x38);	 my_delay(1);
	LCD_Cmd(0x01);	 my_delay(1);
	LCD_Cmd(0x0e);	 my_delay(1);
}
char s[]="India is great";
int i; 
void main()
{	  LCD_init();
	while (1){
		 for ( i=0;i<14;i++){
			LCD_Data( s[i] ); 
		 }
		 my_delay(50);
		LCD_Cmd(0x01); // clear screen
	}

}
/* version 1
		LCD_Cmd(0x38);	 my_delay(1);
		LCD_Cmd(0x38);	 my_delay(1);
		LCD_Cmd(0x01);	 my_delay(1);
		LCD_Cmd(0x0e);	 my_delay(1);
		LCD_Data('I');	 my_delay(1);
		
	   version 2
	   	LCD_Cmd(0x38);	 
		LCD_Cmd(0x38);	 
		LCD_Cmd(0x01);	 
		LCD_Cmd(0x0e);	 
		while(1){
			LCD_Data('I');
			LCD_Data('n');
			LCD_Data('d');	 my_delay(1);
		}
		*/
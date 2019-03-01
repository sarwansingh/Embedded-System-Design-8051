#include<reg51.H>
#define LCDPORT P2
sbit RS=P3^0;
sbit E =P3^1;
sbit C1=P3^4;  sbit C2=P3^3; sbit C3=P3^2;
sbit R1=P3^5;  sbit R2=P3^6; sbit R3=P3^7;

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
int n=0,cnt=0,rishi=1;
void main()
{	// initialize the LCD 
	LCD_Cmd (0x38); LCD_Cmd (0x01);  LCD_Cmd (0x0e);
	C1=C2=C3=R1=R2=R3=1	;
	while(rishi==1){	 
		C1=0;
	   if (	R1==0){ LCD_Data ( '1');n=n*10+1; my_delay(1); while(R1==0);}  
	   if (	R2==0){ LCD_Data ( '4');n=n*10+4;my_delay(1); } 
	   if (	R3==0){ LCD_Data ( '='); my_delay(1); } 
	   C1=1;C2=0;
	   if (	R1==0){ LCD_Data ( '2'); n=n*10+2;my_delay(1); }  
	   if (	R2==0){ LCD_Data ( '5'); n=n*10+5;my_delay(1); } 
	   if (	R3==0){ LCD_Data ( '8'); n=n*10+8; my_delay(1); } 
		C2=1; C3=0;
	   if (	R1==0){ LCD_Data ( '3'); n=n*10+3; my_delay(1); }  
	   if (	R2==0){ LCD_Data ( '6'); n=n*10+6;my_delay(1); } 
	   if (	R3==0){ if (n==123) {
	   					LCD_Data ( 'o');LCD_Data ( 'k');
	   					rishi=2;while(1){} 
					}
	   				else {LCD_Data ( 'n');LCD_Data ( 'o');  cnt++;}
					 my_delay(1); 
					LCD_Cmd (0x01); // clear screen
					n=0;
					if (cnt>2){
						LCD_Data ( 'L');LCD_Data ( 'o');
						LCD_Data ( 'c');LCD_Data ( 'k');
					}
				} 
		C3=1;
	}
	 
}
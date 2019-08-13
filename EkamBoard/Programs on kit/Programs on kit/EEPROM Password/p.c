#include<reg51.h>
#include<intrins.h>  // pata karo ji 
sbit RS = P1^0;
sbit E  = P1^2;
sbit RW = P1^1; 

sbit sda= P1^4;
sbit scl= P1^3;
sbit BUZZER = P3^7;

sbit R1 = P2^4;
sbit R2 = P2^5;
sbit R3 = P2^6;
sbit R4 = P2^7;

sbit C1=  P2^3;
sbit C2=  P2^2;
sbit C3=  P2^1;
sbit C4=  P2^0;

bit ack; 
void mydelay()
{		   int i;
 	for(i=0;i<30000;i++);
}
void lcd_data(char x)
{	RS = 1;	  	E=1;
	P0 =x;	mydelay();
	E=0; 	
}
void lcd_cmd(char x)
{	RS =0 ;	  	E=1;
	P0 =x;	mydelay();
	E=0; 	
}
void lcd_init()
{
 	lcd_cmd(0x38);
	lcd_cmd(0x01);
	lcd_cmd(0x0e);
}
void lcd_str(char s[])
{	int i=0;
	for (i=0;s[i] != '\0' ;i++) lcd_data(s[i]); 	
}
///////////////// i2c wala kaaam
void aknowledge()	  //acknowledge condition
{	scl=1;	_nop_();	_nop_();	scl=0;}

void start()		//start condition
{	
	sda=1;	scl=1;	
	_nop_();    _nop_();	     //No operation
 	sda=0;	scl=0;
}
void stop()			//stop condition
{	
	sda=0;	scl=1;	
	_nop_();  _nop_(); 	
	sda=1;	scl=0;
}


void send_byte(unsigned char x)
{	int i ;
	for (i=0;i<8;i++)
	{	sda = x/128;				 
		x=x<<1; 
		scl=1;		_nop_();		_nop_();	
		scl=0;	
	}  
	 ack=sda;					//reading acknowledge  
	 sda=0;
}
void write_i2c(unsigned char a)
{
 	 start();
	 send_byte(0xA0);	aknowledge();	// address of device 
	 send_byte(0x00);	aknowledge();// device location 
	 send_byte(a);		aknowledge();// write data at that location  
	 stop();  
	 if(ack==0)	  	// data written successfully 
	 {  	lcd_data('o');
			lcd_data('k');	
	 }  
	 aknowledge();
}
unsigned char read_byte()			//reading from EEPROM serially
{	unsigned char reead=0;	
	unsigned int i;	sda=1;
	for(i=0;i<8;i++)	{		
		reead=reead<<1;		
		scl=1;		_nop_();	_nop_();	
		if(sda==1)			reead++;		
		scl=0;	
	}								 //0101 0001
	sda=0;	return reead;				//Returns 8 bit data here
}					  // reead =001010001  	  =81
int  read_i2c()
{		unsigned  char j; 
	start();	
		send_byte(0xA0);	aknowledge();		//device address
		send_byte(0x00);	aknowledge();		//word address
	start();	
		send_byte(0xA1);	//device address	
		aknowledge();	
									   
	j=read_byte();	aknowledge();  	//lcd_data(j);
	stop();	return(j);
}

/////////////////// kaaam khatam
char name[] = "Do google..." ; 
void main()
{	  	int pass=251,i ,n=0; 
		RW = 0;  
		BUZZER = 0; 
		 lcd_init()	;
		 lcd_str("  welcome  " ) ;   	
		lcd_cmd(0x01);
	
		//lcd_str(" Writing...  " ) ;   
 	    //write_i2c(pass);
		 
		lcd_cmd(0xc0);
		//lcd_str(" Reading...  " ) ; 
		i=read_i2c() ;

		// extracting digits  
//		lcd_data(i/100+48); 
//		lcd_data((i/10)%10+48);
//		lcd_data(i%10+48);
	lcd_cmd(0x01); lcd_str(" Enter password" ) ;
	lcd_cmd(0xc0); 
 	R1=R2=R3=R4=1;  C1=C2=C3=C4=1; 
  	while(1){
 		 R1=0; 
		 if (C1==0){ lcd_data('4') ; n=n*10+4; while(C1==0);} 
		 if (C2==0){ lcd_data('3') ; n=n*10+3;while(C2==0);} 
		 if (C3==0){ lcd_data('2') ; n=n*10+2;while(C3==0);} 
		 if (C4==0){ BUZZER = 1; mydelay();mydelay(); BUZZER = 0;
		 		lcd_data('1') ; n=n*10+1; 	while(C4==0);	} 
		 R1=1; R2=0; 
		if (C1==0){ lcd_data('8') ; n=n*10+8; while(C1==0);} 
		if (C2==0){ lcd_data('7') ; n=n*10+7; while(C2==0);} 
		if (C3==0){ lcd_data('6') ; n=n*10+6; while(C3==0);} 
		if (C4==0){ lcd_data('5') ;	n=n*10+5; while(C4==0);} 

		R2=1; R3=0; 
		if (C1==0){ lcd_data('c') ; ; while(C1==0);} 
		if (C2==0){ lcd_data('b') ; while(C2==0);} 
		if (C3==0){ lcd_data('a') ; while(C3==0);} 
		if (C4==0){ lcd_data('9') ;n=n*10+9;	while(C4==0);} 

		 R3=1; R4=0; 
		if (C1==0){  //enter is pressed 
					  lcd_cmd(0x01);
				if (n==i){ lcd_str("pass correct"); } 
				else { lcd_str(" Incorrect  "); } 
					n=0;  while(C1==0);
		} 
		if (C2==0){ 
			write_i2c(72);
			lcd_str("pass changed "); 
			while(C2==0);
			i=read_i2c() ;
		} 
		if (C3==0){ lcd_data('e') ; while(C3==0);} 
		if (C4==0){ lcd_data('d') ;	while(C4==0);} 

		R1=R2=R3=R4=1; 
	}

}

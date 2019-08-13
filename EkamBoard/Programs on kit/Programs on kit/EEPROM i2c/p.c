#include<reg51.h>
#include<intrins.h>
sbit RS = P1^0;
sbit E  = P1^2;
sbit RW = P1^1; 

sbit sda= P1^4;
sbit scl= P1^3;
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
char name[] = "hello I2c" ; 

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
	 send_byte('o');		aknowledge();
 	 send_byte('s');		aknowledge();
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
void read_i2c()
{		unsigned  char j; 
	start();	
		send_byte(0xA0);	aknowledge();		//device address
		send_byte(0x00);	aknowledge();		//word address
	start();	
		send_byte(0xA1);	//device address	
		aknowledge();	
	
 
	j=read_byte();	aknowledge();  	lcd_data(j);
	j=read_byte();	aknowledge();  	lcd_data(j);
	j=read_byte();	aknowledge();  	lcd_data(j);
	 
	stop();
}

/////////////////// kaaam khatam
sbit BUZZER = P3^7; 
void main()
{	  	int i;
	RW = 0;  
	BUZZER = 0;
	lcd_init();	
	for (i=0;name[i] != '\0' ;i++ )	lcd_data(name[i]); 
	lcd_cmd(0xc0);

	write_i2c('C'); 
	read_i2c();
//	while(1)
//	{
//
//	
//	}

}
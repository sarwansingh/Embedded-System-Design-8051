// interfacing ds1307 with AT89C51
#include<reg51.h>
void _nop_(void);
#define delay_us _nop_(); //generates 1 microsecond

#define LCD P0

sbit RS=P1^0;  
sbit EN=P1^2;  
sbit RW=P1^1;  

sbit SCK=P1^6; //serial clock pin
sbit SDA=P1^7; //serial data pin
sbit BUZZER = P3^7;
#define SCKHIGH  SCK=1;
#define SCKLOW   SCK=0;
#define SDAHIGH  SDA=1;
#define SDALOW   SDA=0;

void integer_lcd(int);
void init_lcd(void);
void cmd_lcd(unsigned char);
void write_lcd(unsigned char);
void display_lcd(unsigned char *);
void delay_ms(unsigned int);

void start(void);
void stop(void);
void send_byte(unsigned char);
unsigned char receive_byte(unsigned char);
void write_i2c(unsigned char,unsigned char,unsigned char);
unsigned char read_i2c(unsigned char,unsigned char);

//clock[]={seconds,minutes,hours,day_of_week,date,month,year};
//unsigned char clock[]={0x00,0x40,0x17,0x06,0x24,0x03,0x06};
unsigned char clock[]={0x00,0x03,0x06,0x01,0x07,0x07,0x0e};
unsigned char *s[]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
unsigned char slave_ack,add=0,c,k;
unsigned int num;
            // Real Time Clock Mode

void main(void)
{
	init_lcd();
	BUZZER =0; add=0;
//	while(add<=6)   //update real time clock
//	{    write_i2c(0xd0,add,clock[add]);
//	     add++;
//    }
 	// stop();

	while(1)
	{
	   c=read_i2c(0xd0,0x02);//read hours register and display on LCD
       write_lcd((c/16)+48);		//	18-	  0001 0010	  bcd 12
       write_lcd((c%16)+48);
       write_lcd(':');

	   c=read_i2c(0xd0,0x01);//read minutes register and display on LCD
       write_lcd((c/16)+48);
       write_lcd((c%16)+48);
       write_lcd(':');
	            

       delay_ms(10);

       c=read_i2c(0xd0,0x00);//read seconds register and display on LCD
	   write_lcd((c/16)+48);
	   write_lcd((c%16)+48);
	   write_lcd(' ');
	   
	   display_lcd(s[read_i2c(0xd0,0x03 )]);//read day register and display

	   cmd_lcd(0xc0);  // go to second line 
	   
	   c=read_i2c(0xd0,0x04);//read date register and display on LCD
	   write_lcd((c/16)+48);
	   write_lcd((c%16)+48);

       write_lcd('/');
	 
	   c=read_i2c(0xd0,0x05);//read month register and display on LCD
       write_lcd((c/16)+48);
       write_lcd((c%16)+48);
       write_lcd('/');
       write_lcd('2');
       write_lcd('0');

       c=read_i2c(0xd0,0x06);//read year register and display on LCD
       write_lcd((c/16)+48);
       write_lcd((c%16)+48);
	   /**/

       delay_ms(100);
	   cmd_lcd(0x01);  // clear the screen 
	}
}
//starts i2c, if both SCK & SDA are idle
void start(void)
{ 
    //check SCK. if SCK busy, return else SCK idle
	if(SCK==0)   return;

    //check SDA. if SDA busy, return else SDA idle
	if(SDA==0)  return;

	SDALOW //data low
	delay_us
	SCKLOW  //clock low   
	delay_us
}          
//stops i2c, releasing the bus
void stop(void)
{        
	SDALOW //data low
	SCKHIGH //clock high
	delay_us
	SDAHIGH //data high
	delay_us
}
//transmits one byte of data to i2c bus
void send_byte(unsigned char c)
{
	unsigned mask=0x80;
    do  //transmits 8 bits
    {
        //set data line accordingly(0 or 1)
		if(c&mask)  SDAHIGH //data high
        else        SDALOW //data low

        SCKHIGH   //clock high
        delay_us
        SCKLOW   //clock low
        delay_us
        mask/=2;  //shift mask
    }while(mask>0);
	SDAHIGH  //release data line for acknowledge
	SCKHIGH  //send clock for acknowledge
	delay_us
	slave_ack=SDA; //read data pin for acknowledge
	SCKLOW  //clock low
	delay_us
}      
//receives one byte of data from i2c bus
unsigned char receive_byte(unsigned char master_ack)
{
	unsigned char c=0,mask=0x80;
    do  //receive 8 bits
    {
                SCKHIGH //clock high
                delay_us
                if(SDA==1) //read data
	                c|=mask;  //store data
	            SCKLOW  //clock low
	            delay_us
	            mask/=2; //shift mask
	}while(mask>0);
	if(master_ack==1)   SDAHIGH //don't acknowledge
	else                SDALOW //acknowledge

	SCKHIGH //clock high
	delay_us
	SCKLOW //clock low
	delay_us
	SDAHIGH //data high
	return c;
}
//writes one byte of data(c) to slave device(device_id) at given address(location)
void write_i2c(unsigned char device_id,unsigned char location,unsigned char c)
{
    do
    {     start();      //starts i2c bus
          send_byte(device_id); //select slave device
          if(slave_ack==1)  //if acknowledge not received, stop i2c bus
		  stop();
    }while(slave_ack==1); //loop until acknowledge is received

	send_byte(location); //send address location
	send_byte(c); //send data to i2c bus
	stop(); //stop i2c bus
	delay_ms(4);
}      
//reads one byte of data(c) from slave device(device_id) at given address(location)
unsigned char read_i2c(unsigned char device_id,unsigned char location)
{
	unsigned char c;
    do
    {
		start();   //starts i2c bus  
        send_byte(device_id); //select slave device
        if(slave_ack==1) //if acknowledge not received, stop i2c bus
           stop();
    }while(slave_ack==1); //loop until acknowledge is received

	send_byte(location);  //send address location     
	stop(); //stop i2c bus
	start(); //starts i2c bus  
	send_byte(device_id+1); //select slave device in read mode
	c=receive_byte(1); //receive data from i2c bus
	stop(); //stop i2c bus
	return c;
}      
//initialize lcd
void init_lcd(void)
{                           
	delay_ms(10); //delay 10 milliseconds
	cmd_lcd(0x0e); //lcd on, cursor on
	delay_ms(10);
	cmd_lcd(0x38); //8 bit initialize, 5x7 character font, 16x2 display
	delay_ms(10);
	cmd_lcd(0x06); //right shift cursor automatically after each character is displayed
	delay_ms(10);
	cmd_lcd(0x01); //clear lcd
	delay_ms(10);
	cmd_lcd (0x80);
}
//transmit command or instruction to lcd
void cmd_lcd(unsigned char c)
{	EN=1;	RW=0;//set enable pin
	RS=0; //clear register select pin
	LCD=c; //load 8 bit data
	EN=0; //clear enable pin
	delay_ms(2); //delay 2 milliseconds
}
//transmit a character to be displayed on lcd
void write_lcd(unsigned char c)
{	EN=1; //set enable pin
	RW=0;	RS=1; //set register select pin
	LCD=c;  //load 8 bit data
	EN=0; //clear enable pin
	delay_ms(2); //delay 2 milliseconds
}
//transmit a string to be displayed on lcd
void display_lcd(unsigned char *s)
{
	while(*s)write_lcd(*s++);
}
//generates delay in milli seconds
void delay_ms(unsigned int i)
{
	unsigned int j;
	while(i-->0)
	{   for(j=0;j<500;j++);	}

}   

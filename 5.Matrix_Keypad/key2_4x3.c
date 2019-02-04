#include<reg51.h>
/*
		C1 		C2		C3
	-----------------------
R1	|	1		2		3
R2	|	4		5		6
R3 	|	7 		8		9
R4 	|	 		0		
R1- P1.0
R2-	P1.1
R3- P1.2
R4- P1.3
C1- P1.4
C2- P1.5
C3- P1.6	
*/
sbit R1 = P1^0 ;
sbit R2 = P1^1 ;
sbit R3 = P1^2  ;
sbit R4 = P1^3  ;
sbit C1 = P1^4  ;
sbit C2 = P1^5  ;
sbit C3 = P1^6  ;
void my_delay(int s){
	int i; 
 	for (;s>0; s--)
		for (i=0;i<30000;i++);
}
void main()
{ 	int i ;
	for (i=0 ;i<10 ; i++ ){  P2=i;  my_delay(1); } 
	while (1) {
	 // matrix keypad logic
	 R1=R2=R3=R4=C1=C2=C3 = 1;
	 C1 = 0;
	 if (R1==0)	P2= 1; // print 1 
	 if (R2==0)	P2= 4; // print 4 
	 if (R3==0)	P2= 7; // print 7 
	 if (R4==0)	P2= 10; // print 10 
	 C1=1; C2=0;
	 if (R1==0)	P2= 2; // print  2
	 if (R2==0)	P2= 5; // print 5
	 if (R3==0)	P2= 8; // print 8
	 if (R4==0)	P2= 0; // print 0 
	 C2=1; C3=0;
	 if (R1==0)	P2= 3; // print  3
	 if (R2==0)	P2= 6; // print 6
	 if (R3==0)	P2= 9; // print 9
	 if (R4==0)	P2= 11; // print 11
	 C3=1;
	 my_delay(4);	
	}
}
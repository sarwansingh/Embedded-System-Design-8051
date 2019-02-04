// first LED interfacing code 
#include <reg51.h> // header file having addresses for each port 

void main()
{ 	int i,j=0; 
	while(1) // infinite loop for each embedded program 
	{	
		for (j=0;j<8;j++){ 
			P2 = ~(1<<j); // 0000 0001 ->	1111 1110
			for (i=0 ; i<32000; i++); // donothing loop to insert delay		
		}
	}
}
/*
P2 = ~1; // 0000 0001 ->	1111 1110
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = ~(1<<1); // 0000 0010  -> 1111 1101
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = ~(1<<2) ;
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = ~(1<<3);
		for (i=0 ; i<32000; i++); // donothing loop to insert delay

P2 = 0xfe; // 1111 1110 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0xfd; // 1111 1101 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0xfb; // 1111 1011 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0xf7; // 1111 0111 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay

		P2 = 0xef; // 1110 1111 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0xdf; // 1101 1111 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0xbf; // 1011 1111 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0x7f; // 0111 1111 
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
*/
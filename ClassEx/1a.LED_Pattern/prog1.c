// first LED interfacing code 
#include <reg51.h> // header file having addresses for each port 

void main()
{ 	int i; 
	while(1) // infinite loop for each embedded program 
	{
		P2 = 0xff; // 1111 1111 assign port 2 with all ones.
		for (i=0 ; i<32000; i++); // donothing loop to insert delay
		P2 = 0x00; // 0000 0000 assign port 2 with all zeros.
		for (i=0 ; i<32000; i++){ } // donothing loop to insert delay
	}
}
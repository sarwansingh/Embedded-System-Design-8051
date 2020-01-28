#include<reg51.h>
sbit buzz = P0^4;
void my_delay()
{	int i,j;
 
	for (j=0;j<3 ;j++){
		for (i=0 ; i<30000 ;i++){
		}
	}
}	
void main()
{
  	while(1){
	buzz=0;  my_delay();
	buzz=1;  my_delay();
	} 
}
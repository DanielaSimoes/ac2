#include <detpic32.h>
void delay(unsigned int);
char send2display(int);
 
int main (void){
 
   TRISB = TRISB & 0xFC00;
   LATBbits.LATB9=1;
   LATBbits.LATB8=0;
   
   int i;
   char c;
   for (i =0 ; i<=15; i++){
   	 	c = send2display(i);
        LATB = (LATB & 0xFF00) | c;
        delay(1000);
    }
    return 0;
}




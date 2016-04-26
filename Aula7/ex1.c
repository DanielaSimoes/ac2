#include<detpic32.h>

int main(void){

  U1BRG = (20000000 + 8*115200 )/ (16*115200) - 1;
  U1MODEbits.BRGH = 0;  // divisão por 16
  U1MODEbits.PDSEL = 00;

  /*
	PDSEL<1:0>: Parity and Data Selection bits
	11 = 9-bit data, no parity
	10 = 8-bit data, odd parity
	01 = 8-bit data, even parity
	00 = 8-bit data, no parity
	*/

  U1MODEbits.STSEL = 0; // número de stop bits - 1

  U1STAbits.UTXEN = 1;
  U1STAbits.URXEN = 1;

  U1MODEbits.ON = 1;

  return 0;


}

#include<detpic32.h>

void delay(unsigned int);
void putc(char);
void puts_(char*);

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

  while(1)
  {

    puts_("String de teste\n");
    delay(1000);
  }

  return 0;

}

void puts_(char *str) {
  int i = 0;
  while (str[i]!='\0') {
    putc(str[i]);
    i++;
  }
}

void putc(char byte2send) {
  while(U1STAbits.UTXBF == 1);  // wait while UTXBF == 1
  U1TXREG = byte2send;  // Copy byte2send to the UxTXREG register
}

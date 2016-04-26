#include<detpic32.h>

void configUart(unsigned int, char, unsigned int);
void delay(unsigned int);
void putc(char);
void puts_(char*);

int main(void){
  configUart(57600, 'N', 1);
  TRISEbits.TRISE0 = 0; // RE0 como saída

  while(1){
    while(U1STAbits.TRMT == 1);
    LATEbits.LATE0 = 0; //Set RE0
    puts_("123456789AB");
    LATEbits.LATE0 = 1; //Reset RE0

  }
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

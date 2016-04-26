#include <detpic32.h>

void configUart(unsigned int, char, unsigned int);
void delay(unsigned int);
void putc(char);
void puts_(char*);

int main(void){

  configUart(600, 'N', 1);

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

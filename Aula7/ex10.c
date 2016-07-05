#include<detpic32.h>

void configUart(unsigned int, char, unsigned int);
void putc(char);
char getc(void);

int main(void){
  configUart(115200, 'N', 1);
  EnableInterrupts();

  while(1);
}

void putc(char byte2send) {
  while(U1STAbits.UTXBF == 1);  // wait while UTXBF == 1
  U1TXREG = byte2send;  // Copy byte2send to the UxTXREG register
}

void _int_(24) isr_uart1(void){
  putc(U1RXREG);
  IFS0bits.U1RXIF = 0;
}

char getc(void){
  if(U1STAbits.OERR == 1)
    U1STAbits.OERR = 0;

  while(U1STAbits.URXDA == 0);

  if(U1STAbits.FERR || U1STAbits.PERR){
      char tmp = U1RXREG;
      return 0;
  }

  return U1RXREG;
}

#include<detpic32.h>

void configUart(unsigned int, char, unsigned int);
void delay(unsigned int);
void putc(char);
void puts_(char*);

int main(void){
  configUart(115200, 'N', 1);
  TRISEbits.TRISE0 = 0; // RE0 como saída

  while(1){
    while(U1STAbits.TRMT == 0);
    LATEbits.LATE0 = 1; //Set RE0
    puts_("123456789AB");
    LATEbits.LATE0 = 0; //Reset RE0

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

// 3 microsegundos a 1 para "12345" - que é o tempo que a função put_s demora, e 440 microsegundos a 0 que é o tempo que o FIFO demora a ficar vazio
// Cada bit ocupa 0 carateres, 8 bits (1 byte) + stop bit + start bit
// A transmissão de de cada carater então será aproximadamente 80 microsegundos (10/115200)
// Quando a string tiver > 9 bits, (8 no FIFO e 1 no transmit SR), o tempo ultrapassará os 80 microsegundos
// Para "123456789AB" teremos 300 microsegundos da função puts_

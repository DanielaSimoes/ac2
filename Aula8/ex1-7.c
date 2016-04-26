#include <detpic32.h>

#define DisableUart1RxInterrupt() IEC0bits.U1RXIE = 0 //receção
#define EnableUart1RxInterrupt() IEC0bits.U1RXIE = 1
#define DisableUart1TxInterrupt() IEC0bits.U1TXIE = 0 //transmissão
#define EnableUart1TxInterrupt() IEC0bits.U1TXIE = 1
#define BUF_SIZE 8
#define INDEX_MASK (BUF_SIZE - 1)

typedef struct{
  unsigned char data[BUF_SIZE];
  unsigned int head;
  unsigned int tail;
  unsigned int count;
} circularBuffer;

int i;

volatile circularBuffer txb; // Transmission buffer
volatile circularBuffer rxb; // Reception buffer

void comDrv_flushRx(void) {
  // Initialize variables of the reception buffer
  rxb.tail = 0;
  rxb.head = 0;
  rxb.count = 0;

  for (i = 0; i < BUF_SIZE; i++) {
    rxb.data[i] = 0;
  }
}

void comDrv_flushTx(void) {
  // Initialize variables of the transmission buffer
  txb.tail = 0;
  txb.head = 0;
  txb.count = 0;

  for (i = 0; i < BUF_SIZE; i++) {
    txb.data[i] = 0;
  }
}

void comDrv_putc(char ch)
{
  while(txb.count == BUF_SIZE); // Wait while buffer is full
  txb.data[txb.tail] = ch; // Copy character to the transmission
                          // buffer at position "tail"
  txb.tail = (txb.tail + 1) & INDEX_MASK; // Increment "tail" index
                                          // (mod.BUF_SIZE)
  DisableUart1TxInterrupt(); // Begin of critical section
  txb.count++;  // Increment "count" variable
  EnableUart1TxInterrupt(); // End of critical section
}

void comDrv_puts(char *s) {
  while (*s!='\0') {
    comDrv_putc(*s);
    *s++;
  }
}

void _int_(24) isr_uart1(void){
  if(IFS0bits.U1TXIF){
    if(txb.count>0){
      U1TXREG = txb.data[txb.head];
      txb.head = (txb.head + 1) & INDEX_MASK;
      txb.count--;
    }
    if(txb.count==0){
      DisableUart1TxInterrupt();
    }
    IFS0bits.U1TXIF = 0;
  }
}

void  comDrv_config(unsigned int baud, char parity, unsigned int Stopbits){
	if(baud<600 || baud > 115200){
		baud = 115200;
	}

	if(parity!='N' && parity!='E' && parity!='O'){
		parity = 'N';
	}else if(Stopbits!=1 && Stopbits!=2){
		Stopbits = 1;
	}

	U1BRG = ((20000000 + 8*baud)/ (16*baud))-1;
	U1MODEbits.BRGH = 0; // divisão por 16

	if(parity=='N'){
		U1MODEbits.PDSEL = 00;
	}else if(parity=='E'){
		U1MODEbits.PDSEL = 01;
	}else if(parity=='O'){
		U1MODEbits.PDSEL = 10;
	}
	/*
	Neste caso, número fixo de 8 bits.
	10 = 8-bit data, odd parity
	01 = 8-bit data, even parity
	00 = 8-bit data, no parity
	*/

  // Stopbits
	U1MODEbits.STSEL = (Stopbits - 1);

	/*
	STSEL: Stop Selection bit
	1 = 2 Stopbits
	0 = 1 Stopbit
	*/

	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;

	U1MODEbits.ON = 1;

	/*** UART configure interrupts ***/
	U1STAbits.URXISEL = 00;
	IPC6bits.U1IP=3;
}


int main(void) {
  comDrv_config(115200,'N',1);// default "pterm" parameters
  comDrv_flushRx();
  comDrv_flushTx();
  EnableInterrupts();
  while(1)
    comDrv_puts("Teste do bloco de transmissao do device driver \n");
}

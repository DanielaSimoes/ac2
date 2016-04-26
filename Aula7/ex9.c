#include<detpic32.h>

void configUart(unsigned int, char, unsigned int);
void putc(char);
char getc(void);

int main(void){
  configUart(115200, 'N', 1);

  while(1){
    putc( getc() );
  }
}

/*
		OERR: Receive Buffer Overrun Error Status bit.
		This bit is set in hardware and can only be cleared (= 0) in software.
		Clearing a previously set OERR bit resets the receiver buffer and RSR to empty state.
		1 = Receive buffer has overflowed
		0 = Receive buffer has not overflowed

    URXDA: Receive Buffer Data Available bit (read-only)
		1 = Receive buffer has data, at least one more character can be read
		0 = Receive buffer is empty

    PERR: Parity Error Status bit (read-only)
		1 = Parity error has been detected for the current character
		0 = Parity error has not been detected

    FERR: Framing Error Status bit (read-only)
		1 = Framing error has been detected for the current character
		0 = Framing error has not been detected

    Receive Buffer (UxRXREG)
    The UART receiver has a 9-bit-wide FIFO receive data buffer that is up to 8 levels deep.
    The UxRXREG is a memory mapped register that provides access to the output of the FIFO.
    It is possible for the FIFO to be full and the next word to begin shifting to the UxRSR register before a buffer overrun occurs.
	*/



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

void putc(char byte2send) {
  while(U1STAbits.UTXBF == 1);  // wait while UTXBF == 1
  U1TXREG = byte2send;  // Copy byte2send to the UxTXREG register
}

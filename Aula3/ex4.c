#include <detpic32.h>

void delay(unsigned int);

int main(void) {
	static const unsigned char codes[] = {0x02, 0x01, 0x40, 0x20, 0x10, 0x04, 0x08};
    // configure RB0-RB9 as outputs
    TRISB=TRISB & 0xFC00;
    int i;
	while(1){
		LATBbits.LATB9=1;
    LATBbits.LATB8=0;	// select display low 
		for(i=0;i < 7; i++) {
            LATB = (LATB & 0xFF00) | codes[i]; // é necessário and ff00 para manter RB8 e RB9 inalteraveis, colocar todo o resto a zero e or do codes[i];
            delay(1000);// wait 1 second
		}
		LATBbits.LATB9=0;
        LATBbits.LATB8=1;	// select display high
        for(i=0;i < 7; i++) {
            LATB = (LATB & 0xFF00) | codes[i]; // é necessário and ff00 para manter RB8 e RB9 inalteraveis, colocar todo o resto a zero e or do codes[i];
            delay(1000);// wait 1 second
        }
	}
    return 0;
}

#include <detpic32.h>

void send2displays(char);
void delay(unsigned int);
unsigned char toBCD(unsigned char);

int main(void) {

	TRISB = TRISB & 0xFC00; /* configurar como saidas */
	LATB = LATB & 0xFC00;

	unsigned char counter = 0;// declare variables
	int i;
	char bcd;
// initialize ports
	while(1){
		i = 0;
		do{
			delay(50);	// wait 50 ms
			bcd = toBCD(counter);
			send2displays(bcd);	// call send2displays with counter value as argument
	} while(++i < 4);

	if(counter==0x3B){
			counter = 0;
			do{
				i = 0;
				do{
					delay(10);
					send2displays(0);
				}while(++i<50);
				/* limpar o display */
				LATB = LATB & 0xFC00;
				/* esperar 500 ms */
				delay(500);
			 }while(++counter<5);
			counter = 0;
		}else{
			counter++;
		}
	}
	return 0;
}

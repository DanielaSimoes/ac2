#include <detpic32.h>
#define CALIBRATION_VALUE 10000
void delay(unsigned int intervals);

int main(void) {
	while(1) {
		resetCoreTimer();
		delay(1);
		printInt(readCoreTimer(), 10 + (10 << 16)); 
		printStr("\r\n");
	}
    return 0;
}

void delay(unsigned int n_intervals){
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--) 
		for(i = CALIBRATION_VALUE; i != 0; i--); 
}


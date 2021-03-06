#include<detpic32.h>
int i = 0, sum = 0, V = 0;
void delay(unsigned int);
void send2displays(unsigned char);
unsigned char toBcd(unsigned char);
int main(void){

        //ADC
        TRISBbits.TRISB14 = 1;
        AD1PCFGbits.PCFG14 = 0;
        AD1CHSbits.CH0SA = 14;
        AD1CON2bits.SMPI = 15;
        AD1CON1bits.SSRC = 7;
        AD1CON1bits.CLRASAM = 1;
        AD1CON3bits.SAMC = 16;
        AD1CON1bits.ON = 1;

        //ADC interrupts
        IPC6bits.AD1IP = 3;
        IEC1bits.AD1IE = 1;
        EnableInterrupts();

        //Timer 2
        PR2 = 49999;
        T2CONbits.TCKPS = 0;
        TMR2 = 0;
        T2CONbits.TON = 1;

        // Timer interrupts
        IFS0bits.T2IF = 0;
        IPC2bits.T2IP = 2;
        IEC0bits.T2IE = 1;

    while (1){
        delay(20);
        AD1CON1bits.ASAM = 1;
    };
}

void _int_(27) isr_adc(void){

    int *p = (int *)(&ADC1BUF0);
    for( i = 0; i < 16; i++ ){
        sum += p[i*4];
    }

    V=((sum/16)*70+511)/1023;

    sum = 0;

    IFS1bits.AD1IF = 0;
}

void _int_(8) isr_T2(void){
    send2displays(toBcd(V));
    IFS0bits.T2IF = 0;
}

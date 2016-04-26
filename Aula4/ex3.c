#include <detpic32.h>

int main(void) {
  volatile int aux;
  // Configure A/D module;

  TRISBbits.TRISB14 = 1; // RB14 digital output disconnected
  AD1PCFGbits.PCFG14 = 0; // RB14 configured as analog input (AN4)

  AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this // modeaninternalcounterendssamplingand
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  AD1CON1bits.ON = 1;
  AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)

  AD1CHSbits.CH0SA = 14;
  AD1CON2bits.SMPI = 0; // uma conversao consecutiva

  TRISEbits.TRISE0 = 0; //configure RE0 as digital output

  while(1)
  {
    LATEbits.LATE0 = 1; // Set RE0
    AD1CON1bits.ASAM = 1;  // Start conversion
    while(IFS1bits.AD1IF == 0);// Wait while conversion not done (AD1IF == 0)
    LATEbits.LATE0 = 0; // Reset RE0
    aux = ADC1BUF0;// Read conversion result (ADC1BUF0) to "aux" variable
    IFS1bits.AD1IF = 0; // Reset AD1IF
  }
}

// valor lido do osciloscópio: aproximadamente 3,6 microsegundos

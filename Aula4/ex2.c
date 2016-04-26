#include <detpic32.h>

int main(void) {
  TRISBbits.TRISB14 = 1; // RB14 disconnected
  AD1PCFGbits.PCFG14 = 0;

  AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this // modeaninternalcounterendssamplingand
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  AD1CON1bits.ON = 1;
  AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)

  AD1CHSbits.CH0SA = 14;
  AD1CON2bits.SMPI = 0; // uma conversao consecutiva

while(1){
    AD1CON1bits.ASAM = 1; // Start conversion
    while(IFS1bits.AD1IF == 0 ); // Wait while conversion not done
    printInt(ADC1BUF0, 16);
    printStr("\n");
    IFS1bits.AD1IF = 0;
  }
  return 0;
}

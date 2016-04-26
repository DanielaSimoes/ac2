#include <detpic32.h>
void delay(unsigned int);
char send2displays(int);
unsigned char toBCD(unsigned char value);

int main(void) {
  // Configure A/D module;
  int i = 0, V = 0;
  double average = 0;
  double sum = 0;

  TRISBbits.TRISB14 = 1; // RB4 digital output disconnected
  AD1PCFGbits.PCFG14 = 0; // RB4 configured as analog input (AN4)

  AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this // modeaninternalcounterendssamplingand
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  AD1CON1bits.ON = 1;
  AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)

  AD1CHSbits.CH0SA = 14;
  AD1CON2bits.SMPI = 3; // uma conversao consecutiva

  while(1){
    delay(10);  // Wait 10 ms using the core timer
    if(i++ == 25) // 250 ms
    {
      AD1CON1bits.ASAM = 1;  // Start conversion
      while(IFS1bits.AD1IF == 0 );// Wait while conversion not done (AD1IF == 0)

      int *p = (int *)(&ADC1BUF0);

      for( i = 0; i < 16; i++ ){
        printInt10(p[i*4]);
        sum += p[i*4];
        printStr("\n");
      }

      average = sum/4;

      printf("%s %f","Média dos valores lidos: ", average );

      V = (average*33+511)/1023;
      printStr("\n");
      printf("%s %d","Tensão: ", V );

      i = 0; sum = 0; average = 0;
      IFS1bits.AD1IF = 0; // Reset AD1IF

      V = toBCD(V);
    }
    send2displays(V);
  }

  return 0;
  // Consoante o número de conversões consecutivas, aparecerão os valores até 15 conversões consecutivas
}

#include <detpic32.h>

volatile unsigned char value2display = 0; // Global variable
int i = 0;
void send2displays(unsigned char);
unsigned char toBCD(unsigned char);
void delay(unsigned int);

int main(void) {
  TRISBbits.TRISB14 = 1; // RB14 digital output disconnected
  AD1PCFGbits.PCFG14 = 0; // RB14 configured as analog input (AN4)

  AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this // modeaninternalcounterendssamplingand
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  AD1CON1bits.ON = 1;
  AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)

  AD1CHSbits.CH0SA = 14;
  AD1CON2bits.SMPI = 7; // uma conversao consecutiva

  IPC6bits.AD1IP = 3; // valor aleatório entre 1 e 6, 0 é inativo e 7 prioridade máxima. Como só há uma interrupção pode ser qualquer valor entre 1 e 6
  IEC1bits.AD1IE = 1; // autorizar interrupções pelo módulo A/D

  IFS1bits.AD1IF = 0; // Quando a conversao estiver concluida, este valor deve ser 1
  EnableInterrupts(); // Autorização globalmente o sistema de interrupções

  i = 0;

  while(1)
  {
    delay(10);  // Wait 10 ms using the core timer
    if(i++ == 25) // 250 ms
    {
      AD1CON1bits.ASAM = 1;  // Start conversion
      i = 0;
    }
    send2displays(value2display);

  }
  return 0;
}

void _int_(27) isr_adc(void) {
  int V = 0;
  int sum = 0;
  double average = 0;

  int *p = (int *)(&ADC1BUF0);

  for( i = 0; i < 8; i++ ){
    printInt10(p[i*4]);
    sum += p[i*4];
    printStr("\n");
  }

  average = sum/8;

  printf("%s %f","Média dos valores lidos: ", average );

  V = (average*33+511)/1023;
  printStr("\n");
  printf("%s %d","Tensão: ", V );

  value2display = toBCD(V);
  IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

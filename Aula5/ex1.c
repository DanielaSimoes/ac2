#include <detpic32.h>
void delay(unsigned int);
char send2displays(int);
unsigned char toBCD(unsigned char value);

int main(void) {
  // Configure A/D module;

  // configurar R14 como porta analógica
  TRISBbits.TRISB14 = 1; // RB4 digital output disconnected
  AD1PCFGbits.PCFG14 = 0; // RB4 configured as analog input (AN4)

  // Sempre preciso
  AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this // modeaninternalcounterendssamplingand
  AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
  AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)

  // Ativar o conversor
  AD1CON1bits.ON = 1;
  AD1CHSbits.CH0SA = 14; // AN14 - causa RB14 - como input para o AD converter
  AD1CON2bits.SMPI = 0; // uma conversao consecutivas

  IPC6bits.AD1IP = 3; // valor aleatório entre 1 e 6, 0 é inativo e 7 prioridade máxima. Como só há uma interrupção pode ser qualquer valor entre 1 e 6
  IEC1bits.AD1IE = 1; // autorizar interrupções pelo módulo A/D

  IFS1bits.AD1IF = 0; // Quando a conversao estiver concluida, este valor deve ser 1
  EnableInterrupts(); // Autorização globalmente o sistema de interrupções
  AD1CON1bits.ASAM = 1;  // Start conversion


  while(1){
    // do nothing
  }
}

// Interrupt Handler
void _int_(27) isr_adc(void){
  // Print ADC1BUF0 value
  printInt(ADC1BUF0, 16);
  putChar('\n');
  AD1CON1bits.ASAM = 1;  // Start conversion
  IFS1bits.AD1IF = 0; // Reset AD1IF flag

}

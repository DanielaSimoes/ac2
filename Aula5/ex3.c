#include <detpic32.h>
// Overhead - tempo para entrar e sair da rotina de interrupção => tempo de latencia + tempo de saida

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

  IPC6bits.AD1IP = 3; // valor aleatório entre 1 e 6, 0 é inativo e 7 prioridade máxima. Como só há uma interrupção pode ser qualquer valor entre 1 e 6
  IEC1bits.AD1IE = 1; // autorizar interrupções pelo módulo A/D

  IFS1bits.AD1IF = 0; // Quando a conversao estiver concluida, este valor deve ser 1
  EnableInterrupts(); // Autorização globalmente o sistema de interrupções
  AD1CON1bits.ASAM = 1;  // Start conversion

  while(1){
    LATEbits.LATE0 = 0;
  }
}

void _int_(27) isr_adc(void){
  // Print ADC1BUF0 value
  printInt(ADC1BUF0, 16);
  putChar('\n');

  AD1CON1bits.ASAM = 1;  // Start conversion
  IFS1bits.AD1IF = 0; // Reset AD1IF flag

  LATEbits.LATE0 = 1;
}

// valor lido do osciloscópio: aproximadamente 1 microsegundo
// frequencia do CPU  40Hz => um ciclo de relógio demora 0,025 segundos => 25000 microsegundos
// (1*1)/25000 = 4*10^-5 ciclos de relógio
// Overhead = 2,2 microsegundos

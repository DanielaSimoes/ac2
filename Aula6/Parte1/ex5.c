#include <detpic32.h>

volatile unsigned char value2display = 0; // Global variable
int i = 0;
void send2displays(unsigned char);
unsigned char toBCD(unsigned char);

int main(void) {

  /*            ADC              */
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

  /*            TIMER 1              */

	T1CONbits.TCKPS = 3;

	PR1 = 19530; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR1 = 0; // Reset timer T3 count register
	T1CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	// configuraçao das interrupts tabela 7.1
	IPC1bits.T1IP = 4; // Configurar a prioridade relativa
	IFS0bits.T1IF = 0; // Reset do Instruction Flag
	IEC0bits.T1IE = 1; // Autorizar a interrupção

  /*            TIMER 3              */

  T3CONbits.TCKPS = 2;

	PR3 = 49999; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	// configuraçao das interrupts tabela 7.1
	IPC3bits.T3IP = 4; // Configurar a prioridade relativa
	IFS0bits.T3IF = 0; // Reset do Instruction Flag
	IEC0bits.T3IE = 1; // Autorizar a interrupção

  i = 0;

  /*      Configurar RE4 e RE5      */
  TRISEbits.TRISE4 = 1;
  TRISEbits.TRISE5 = 1;

  while(1){
    if(PORTEbits.RE4==1 && PORTEbits.RE5==0){
			IEC0bits.T1IE = 0; // enable
		}else{
			IEC0bits.T1IE = 1; // enable
		}
  };

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

void _int_(4) isr_T1(void) {
  AD1CON1bits.ASAM = 1; // Start A/D conversion
  IFS0bits.T1IF = 0;  // Reset T1IF flag
}

void _int_(12) isr_T3(void) {
  send2displays(value2display); // Send "value2display" global variable to displays
  IFS0bits.T3IF = 0;  // Reset T3IF flag
}

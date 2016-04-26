#include <detpic32.h>

volatile unsigned char value2display = 0; // Global variable
int i = 0;
void send2displays(unsigned char);
unsigned char toBCD(unsigned char);
void configUart(unsigned int, char, unsigned int);
void puts_(char*);
void setPWM(unsigned int);
void putc(char);

int voltMin = 100;
int voltMax = 0;

int main(void) {
  const static unsigned char pwmValues[]={3, 15, 40, 90};

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

  /*            TIMER 3              */

  T3CONbits.TCKPS = 2;

	PR3 = 49999; // Fout = 20MHz / (32 * (39061,5 + 1)) = 2 Hz
	TMR3 = 0; // Reset timer T3 count register
	T3CONbits.TON = 1; // Enable timer T3 (must be the last command of the // timer configuration sequence)

	// configuraçao das interrupts tabela 7.1
	IPC3bits.T3IP = 4; // Configurar a prioridade relativa
	IFS0bits.T3IF = 0; // Reset do Instruction Flag
	IEC0bits.T3IE = 1; // Autorizar a interrupção

  /*  UART1   */
  configUart(115200,'N',1);

  OC1CONbits.OCM = 6; // ativação do modo PWM
  OC1CONbits.OCTSEL = 1; // 1 ou 0 - ver página 16.8
  OC1CONbits.ON = 1; // ativação do módulo

  EnableInterrupts(); // Autorização globalmente o sistema de interrupções
  AD1CON1bits.ASAM = 1;

  while(1){
    int portval = (PORTE >> 4) & 0x3;
    switch (portval) {
      case 0:
        IEC0bits.T1IE = 1;
        setPWM(0);
        break;
      case 1:
        IEC0bits.T1IE = 1;
        setPWM(100);
        break;
      case 2:
        IEC0bits.T1IE = 0;
        int dc = (PORTE >> 6) & 0x3;
        setPWM(pwmValues[dc]);
        break;
    }
    IEC0bits.T1IE = 0;
  };

  while(1);

  return 0;
}

void _int_(27) isr_adc(void) {
  int V = 0;
  int sum = 0;
  double average = 0;


  int *p = (int *)(&ADC1BUF0);

  for( i = 0; i < 8; i++ ){
    sum += p[i*4];
  }

  average = sum/8;

  V = (average*33+511)/1023;

  if(V>voltMax){
    voltMax = (V);
  }

  if(V<=voltMin){
    voltMin = (V);
  }

  value2display = (V);
  AD1CON1bits.ASAM = 1;
  IFS1bits.AD1IF = 0; // Reset AD1IF flag
}

void _int_(12) isr_T3(void) {
  static int counter = 0;
  printf("%s%d\n", "value2display: ", value2display);
  send2displays(value2display); // Send "value2display" global variable to displays
  if(++counter==100){
    counter = 0;
    puts_("V: ");
    putc(value2display/10 + 48);
    putc((value2display - (value2display/10)*10) + 48);
    putc('\n');
  }
  IFS0bits.T3IF = 0;  // Reset T3IF flag
}


void _int_(24) isr_uart1(void){

  if(IFS0bits.U1EIF){
		if(U1STAbits.OERR == 1){
   			U1STAbits.OERR = 0;
   		}else{
   			int dummy = U1RXREG;
   		}
   		IFS0bits.U1EIF = 0;
	}

  if(U1RXREG == 'l'){
    puts_("voltMin: ");
    putc(voltMin/10 + 48);
    putc((voltMin - (voltMin/10)*10) + 48);
    putc('\n');
    puts_("voltMax: ");
    putc(voltMax/10 + 48);
    putc((voltMax - (voltMax/10)*10) + 48);
    putc('\n');

  IFS0bits.U1RXIF = 0;
  }
}

void puts_(char *str) {
  int i = 0;
  while (str[i]!='\0') {
    putc(str[i]);
    i++;
  }
}

void setPWM(unsigned int dutyCycle){
	OC1RS = ((PR3+1)*dutyCycle)/100; // programação da constante ON
}

void putc(char byte2send) {
  while(U1STAbits.UTXBF == 1);  // wait while UTXBF == 1
  U1TXREG = byte2send;  // Copy byte2send to the UxTXREG register
}

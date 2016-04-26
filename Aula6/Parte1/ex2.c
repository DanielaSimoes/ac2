#include<detpic32.h>

int main(void)
{
  T3CONbits.TCKPS = 8; // 1:256 prescaler (i.e fin = 78125 Hz)
  PR3 = 39062;  // Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
  TMR3 = 0; // Reset timer T3 count register
  T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timerconfigurationsequence)

  IPC3bits.T3IP = 4; // Configurar a prioridade relativa
	IFS0bits.T3IF = 0; // Reset do Instruction Flag
	IEC0bits.T3IE = 1; // Autorizar a interrupção

  EnableInterrupts();
  while(1);

  return 0;
}
void _int_(12) isr_T3(void)// Replace VECTOR by the timer T3 // vectornumber
{
  putChar('.');
  IFS0bits.T3IF = 0;  // Reset T3 interrupt flag
}

#include<detpic32.h>

int main(void) {
  // Configure Timer T3 (2 Hz, interrupts disabled)

  // 1º calcular Fin => 20 MHz / constante.
  // constante = 2, 4, 8, 16 ... 256
  // calcular PR2 => ( valor calculado / frequencia escolhida ) - 1
  // este valor tem de ser menor que 65535

  T3CONbits.TCKPS = 8; // 1:256 prescaler (i.e fin = 78125 Hz)
  PR3 = 39062;  // Fout = 20MHz / (256 * (39062 + 1)) = 2 Hz
  TMR3 = 0; // Reset timer T3 count register
  T3CONbits.TON = 1;  // Enable timer T3 (must be the last command of the timerconfigurationsequence)

  while(1)
  {
    while(IFS0bits.T3IF == 0);  // Wait until T3IF == 1
    IFS0bits.T3IF = 0;  // Reset T3IF
    putChar('.');
  }

  return 0;
}

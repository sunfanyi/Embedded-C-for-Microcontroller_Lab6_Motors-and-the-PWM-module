// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "rc_servo.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    Timer0_init();
    Interrupts_init();
	
    TRISDbits.TRISD5 = 0;
    PWM_PIN = 0;
            
//    TRISHbits.TRISH3 = 0;  // testing
//    LATHbits.LATH3 = 0;
//    TRISDbits.TRISD7 = 0;
//    LATDbits.LATD7 = 0;
    
    unsigned int angle = 0;
    while(1){
        while (angle <= 180) {
            angle2PWM(angle++);
            __delay_ms(50);
        }
        while (angle >= 0) {
            angle2PWM(angle--);
            __delay_ms(50);
        }
    }
}
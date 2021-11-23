// CONFIG1L
#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG3L
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
//#include "rc_servo.h"
#include "dc_motor.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void){
    initDCmotorsPWM(10);  // 10kHz PWMfreq
    
    unsigned char PWMcycle = T2PR;
    struct DC_motor motorL, motorR; 		//declare two DC_motor structures 
    
    motorL.power = 0; 						//zero power to start
    motorL.direction = 1; 					//set default motor direction
    motorL.dutyHighByte = (unsigned char *)(&PWM6DCH);	//store address of PWM duty high byte
    motorL.dir_LAT = (unsigned char *)(&LATE); 		//store address of LAT
    motorL.dir_pin = 4; 						//pin RE4 controls direction
    motorL.PWMperiod = PWMcycle; 			//store PWMperiod for motor
    
    motorR.power = 0; 						//zero power to start
    motorR.direction = 1; 					//set default motor direction
    motorR.dutyHighByte = (unsigned char *)(&PWM7DCH);	//store address of PWM duty high byte
    motorR.dir_LAT = (unsigned char *)(&LATG); 		//store address of LAT
    motorR.dir_pin = 6; 						//pin RC7 controls direction
    motorR.PWMperiod = PWMcycle; 			//store PWMperiod for motor
    
    TRISHbits.TRISH3 = 0;  // testing
    LATHbits.LATH3 = 0;
    TRISDbits.TRISD7 = 0;
    LATDbits.LATD7 = 0;
    
    while(1){
        fullSpeedAhead(&motorL, &motorR, 1);  // forward
        __delay_ms(1000);
//        LATHbits.LATH3 = 1;
        
        stop(&motorL, &motorR);
        __delay_ms(1000);
//        LATDbits.LATD7 = 1;
        
        fullSpeedAhead(&motorL, &motorR, 0);  // backward
        __delay_ms(1000);
        stop(&motorL, &motorR);
        __delay_ms(1000);
    }
}
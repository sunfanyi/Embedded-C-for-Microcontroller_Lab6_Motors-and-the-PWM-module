#include <xc.h>
#include "dc_motor.h"

// function initialise T2 and PWM for DC motor control
void initDCmotorsPWM(unsigned char PWMfreq){
	//initialise your TRIS and LAT registers for PWM
    TRISEbits.TRISE2=0;
    TRISEbits.TRISE4=0;
    TRISCbits.TRISC7=0;
    TRISGbits.TRISG6=0;
    
    LATEbits.LATE2=0;  // power pin
    LATEbits.LATE4=1;  // direction pin
    LATCbits.LATC7=0;
    LATGbits.LATG7=1;
    
    // timer 2 config
    // PS/16e6 * 255 > 1/5kHz  // compatible for any PWNfreq above 5kHz
    T2CONbits.CKPS = 0b100; // 1:16 prescaler
    T2HLTbits.MODE = 0b00000; // Free Running Mode, software gate only
    T2CLKCONbits.CS = 0b0001; // Fosc/4

    // Tpwm*(Fosc/4)/prescaler - 1 = PTPER
    // 1/PWMfreq * (Fosc/4)/prescaler - 1 = PTPER
    T2PR = 16000 / PWMfreq / 16; //Period reg based on PWNfreq
    T2CONbits.ON = 1;
    
    RE2PPS=0x0A; //PWM6 on RE2
    RC7PPS=0x0B; //PMW7 on RC7

    PWM6DCH=0; //0% power
    PWM7DCH=0; //0% power
    
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
}


// function to set PWM output from the values in the motor structure
void setMotorPWM(struct DC_motor *m)
{
	int PWMduty; //tmp variable to store PWM duty cycle

	if (m->direction){ //if forward
		// low time increases with power
		PWMduty=m->PWMperiod - ((int)(m->power)*(m->PWMperiod))/100;
	}
	else { //if reverse
		// high time increases with power
		PWMduty=((int)(m->power)*(m->PWMperiod))/100;
	}

	*(m->dutyHighByte) = PWMduty; //set high duty cycle byte
        
	if (m->direction){ // if direction is high
		*(m->dir_LAT) = *(m->dir_LAT) | (1<<(m->dir_pin)); // set dir_pin bit in LAT to high without changing other bits
	} else {
		*(m->dir_LAT) = *(m->dir_LAT) & (~(1<<(m->dir_pin))); // set dir_pin bit in LAT to low without changing other bits
	}
}

//function to stop the robot gradually 
void stop(struct DC_motor *mL, struct DC_motor *mR)
{
    while (mL->power | mR->power) {  // only stop when both reach 0
        // decrement power, 10% each time
        if (mL->power) {mL->power -= 10;}
        if (mR->power) {mR->power -= 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);
    }
}

//function to make the robot turn left 
void turnLeft(struct DC_motor *mL, struct DC_motor *mR)
{

}

//function to make the robot turn right 
void turnRight(struct DC_motor *mL, struct DC_motor *mR)
{
 
}

//function to make the robot go straight
void fullSpeedAhead(struct DC_motor *mL, struct DC_motor *mR,
        unsigned char direction)
{
    mL->direction = direction;
    mR->direction = direction;
    while ( (!(mL->power==100)) | (!(mR->power==100)) ) {  // only stop when both reach max power
        // increment power, 10% each time
        if (!(mL->power==100)) {mL->power += 10;}
        if (!(mR->power==100)) {mR->power += 10;}
        setMotorPWM(mL);
        setMotorPWM(mR);
        __delay_ms(100);
    }
}
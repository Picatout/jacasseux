/*
 * File:   blink.c
 * Author: jacques
 *
 * Created on 18 octobre 2020, 13:08
 */

// CONFIG
#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#pragma config LPBOR = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

#include <xc.h>

#define NCO_CAL (16384)
#define FREQ (8258)
#define AUDIO_P (1<<1)  // RA1
#define CAP_P (1<<2)    // RA2


void msec_delay(unsigned int msec){
    while(msec--){
        // délais
        asm("CLRF TMR0");
        asm("BCF INTCON,2");
        asm("BTFSS INTCON,2");
        asm("GOTO $-1");
   }
}

unsigned int read_cap(){
    unsigned int cap_val;
        TMR0=0;
        TRISA|=CAP_P;
        asm("btfss PORTA,2");
        asm("goto $-1");
        cap_val=TMR0;
        TRISA&=~CAP_P;
        LATA&=~CAP_P;
        return cap_val;
}

void wait_trigger(){
    asm(
    "clrf IOCAF\n"
    "btfss IOCAF,0\n"
    "goto $-1\n"
    );
}

void main(void) {
    unsigned int babble_time;
    OSCCON|=(7<<4);
    ANSELA=0;
    OPTION_REG=1;
    TRISA&=~(AUDIO_P|CAP_P);
    LATA&=~(AUDIO_P|CAP_P);
    PORTA&=~(CAP_P);
    WPUA|=CAP_P;
    IOCAP=1;
    IOCAN=0;
    // initialisation PWM CH1
    PWM1CON=(1<<7); 
    PR2=31;
    PWM1DCH=16;
    PWM1DCL=0;
    T2CON=(1<<2);
    // initialisation NCO
    NCO1CLK=2;
    NCO1CON=0x80;
    // initialisation CLC
    CLC1GLS0=2;
    CLC1GLS1=0;
    CLC1GLS2=0x20;
    CLC1GLS3=0;
    CLC1SEL0=3;
    CLC1SEL1=5;
    CLC1POL=0;
    CLC1CON=0xC1;
    while (1){
        NCO1INC=NCO_CAL;
        wait_trigger();
        babble_time=1000;
        while (babble_time--){
            NCO1INC=read_cap()+NCO_CAL;
            msec_delay(100);
        }
    }
    return;
}

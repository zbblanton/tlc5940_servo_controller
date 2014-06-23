/*
 * File:   main.c
 * Author: Zachary Blanton (zbblanton@gmail.com)
 *
 * Created on December 17, 2013, 11:27 PM
 */

//Microcontroller: PIC18F4520
//8MHz internal clock
//Complier: xc8 v1.32
#include <xc.h>
#include "tlc5940.h"

#define _XTAL_FREQ 8000000
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

#define OSCCON = 0b01110000
#define ANSEL = 0b00000000
#pragma config LVP = OFF

#pragma config OSC = HS, WDT=OFF, PWRT=OFF, BOREN=OFF, CP0=OFF,CP1=OFF,CP2=OFF,CP3=OFF, MCLRE=ON

volatile int count;

void delay_sec() //actually, half a second
{
    for(int i = 0; i < 50; i++)
    {
        tlc_delay_ms(10);
    }
}

void interrupt blank_pulse(void)
{
    if(PIE1bits.TMR2IE && PIR1bits.TMR2IF)
    {
        if(count >= 125) //572 520
        {
            tlc_blank = 1;
            tlc_delay_us(1);
            tlc_blank = 0;
            count = 0; //Reset count
        }
        count++;
        PIR1bits.TMR2IF = 0;
    }
}

void spi_init()
{
    SSPSTATbits.SMP = 0; //Input data sampled at middle of data output time
    SSPSTATbits.CKE = 1; //Transmit on transition from active to Idle clock state
    SSPCON1bits.CKP = 0; //Idle state for clock is a low level
    SSPCON1bits.SSPM0 = 0;//Sets SPI in master mode and sets clock to FOSC/4
    SSPCON1bits.SSPM1 = 0;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM3 = 0;
    SSPCON1bits.SSPEN = 1; //Enables the serial port

    TRISCbits.RC5 = 0; //Set SDO to ouput
    TRISCbits.RC3 = 0; //Set SCK to output
}

void TMR2_init()
{
    //Set to 100,000hz with 50% duty cycle. Prescaler is 4. Post scale is 16.
    PR2 = 0b00000100;
    T2CON = 0b01111101;
    CCPR1L = 0b00000010;
    CCP1CON = 0b00101100;
    TRISC = 0b00000000;
    PORTC = 0b00000000;
    T2CONbits.TMR2ON = 0; //Turn Timer off
}

int main()
{
    TRISD = 0b00000000;
    PORTD = 0b00000000;
    TRISB = 0b00100000;
    PORTB = 0b00000000;
    TRISA = 0b00001111;
    PORTA = 0b00001111;
    TRISE = 0b00001000;
    PORTE = 0b00000000;

    TMR2_init();
    count = 0;

    PIE1bits.TMR2IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    T2CONbits.TMR2ON = 0; //Turn Timer on

    spi_init();
    //init_dot_correction();
    //init_grayscale();
    tlc_init();
    tlc_update();

    T2CONbits.TMR2ON = 1; //Turn Timer on
    while(1)
    {
        LATDbits.LD3 = 1;
        delay_sec();
        LATDbits.LD3 = 0;
        delay_sec();
        tlc_set(0, 0);
        tlc_update();
        delay_sec();
        delay_sec();
        delay_sec();
        delay_sec();

        tlc_set(0, 180);
        tlc_update();
        delay_sec();
        delay_sec();
        delay_sec();
        delay_sec();
    }
}
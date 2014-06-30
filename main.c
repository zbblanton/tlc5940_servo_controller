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

    count = 0;
    tlc_TMR2_init();
    tlc_spi_init();
    tlc_init();

    while(1)
    {
        LATDbits.LD3 = 1;
        delay_sec();
        LATDbits.LD3 = 0;
        delay_sec();

        //test code
        tlc_set(15, 175);
        tlc_set(14, 130);
        tlc_set(13, 90);
        tlc_update();

        delay_sec();
        delay_sec();
        delay_sec();
        delay_sec();

        //test code
        tlc_set(15, 175);
        tlc_set(14, 115);
        tlc_set(13, 110);
        tlc_update();

        delay_sec();
        delay_sec();
        delay_sec();
        delay_sec();

        tlc_sweep_set(15,160);
        tlc_sweep_set(14,100);
        tlc_sweep_set(13,30);
        tlc_sweep_update(5);

        delay_sec();
        delay_sec();
        delay_sec();
        delay_sec();

        tlc_write(14, 125);
        delay_sec();
        delay_sec();
        //end
    }
}
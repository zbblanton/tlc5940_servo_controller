/*
 * File:   tlc5940.c
 * Author: Zachary Blanton (zbblanton@gmail.com)
 *
 * Created on December 17, 2013, 11:27 PM
 */

#include "tlc5940.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000
#endif
#ifndef __delay_us(x)
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#endif
#ifndef __delay_ms(x)
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
#endif


//char tlc_servo[16];
//char tlc_servo_temp[16];


void tlc_spi_init()
{
    SSPSTATbits.SMP = 0; //Input data sampled at middle of data output time
    SSPSTATbits.CKE = 1; //Transmit on transition from active to Idle clock state
    SSPCON1bits.CKP = 0; //Idle state for clock is a low level
    SSPCON1bits.SSPM0 = 0;//Sets SPI in master mode and sets clock to FOSC/4
    SSPCON1bits.SSPM1 = 0;
    SSPCON1bits.SSPM2 = 0;
    SSPCON1bits.SSPM3 = 0;
    SSPCON1bits.SSPEN = 1; //Enables the serial port

    tlc_spi_output_io = 0; //Set SDO to ouput
    tlc_spi_clock_io = 0; //Set SCK to output
}

void tlc_spi_send_data(char data)
{
    SSPBUF = data; //Loads data into SSPBUF reg
    tlc_delay_us(2); //Testing a much faster delay
}

void tlc_pwm_init()
{
    //Set to 100,000hz with 50% duty cycle. Prescaler is 4. Post scale is 16.
    PR2 = 0b00000100;
    T2CON = 0b01111101;
    CCPR1L = 0b00000010;
    CCP1CON = 0b00101100;
    tlc_pwm_io = 0;

    PIE1bits.TMR2IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    T2CONbits.TMR2ON = 0; //Turn Timer off
}

/**************************************************************************
Macro       : tlc_init

Description : Set the TLC5940 up for operation.

Arguments   : None.

Remarks     : This function sends the inital dot correction data and the
initial position data set by the user to the TLC5940. 
***************************************************************************/
void tlc_init()
{
    //Initialize interrupt counter
    tlc_interrupt_counter = 0;

    //Set the pins to output
    tlc_xlat_io = 0;
    tlc_vprg_io = 0;
    tlc_blank_io = 0;

    //Initialize dot correction
    tlc_vprg_pin = 1; //Dot correction input mode
    tlc_xlat_pin = 0;
    tlc_delay_ms(2);

    //Send initial dot correction data
    for(char i = 0; i < 12; i++)
    {
        tlc_send_data(0b11111111);
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat_pin = 1;
    tlc_delay_us(1);
    tlc_xlat_pin = 0;
    tlc_delay_us(1);
    //end

    //Initialize grayscale
    tlc_delay_ms(2);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg_pin = 0; //Grayscale input mode
    tlc_xlat_pin = 0;
    tlc_blank_pin = 0;

    //Initialize the default servo position
    for(int i = 0; i < 16; i++)
    {
        if(tlc_servo[i] != 0)
        {
            tlc_servo[i] = 0;
        }
        tlc_servo_temp[i] = tlc_servo[i];
    }

    //Copy initial servo positions to temp
//    for(int i = 0; i < 16; i++)
//    {
//        tlc_servo_temp[i] = tlc_servo[i];
//    }

    //Send default servo positions.
    char counter = 0;
    for(char i = 15; i > 7; i--)
    {
        tlc_send_data(tlc_servo[i - counter]>>4);
        tlc_send_data(tlc_servo[i - counter]<<4);
        tlc_send_data(tlc_servo[i - (counter + 1)]);
        counter++;
    }

    //Testing faster toggle speeds
    tlc_delay_us(3);
    tlc_xlat_pin = 1;
    tlc_delay_us(1);
    tlc_xlat_pin = 0;
    tlc_delay_us(1);

    tlc_send_data(0b10000000);//one clock pulse for first grayscale
    //end

    T2CONbits.TMR2ON = 1;
}

/**************************************************************************
Macro       : tlc_update

Description : Updates the position on the selected servo.

Arguments   : None.

Remarks     : This function sends the new position data to the TLC5940. It
turns the timer off and sets the tlc pins and then begins to send the data.
***************************************************************************/
void tlc_update()
{   
    //tlc_delay_ms(2);
    tlc_delay_us(1);

    //T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg_pin = 0; //Grayscale input mode
    tlc_xlat_pin = 0;
    tlc_blank_pin = 0;

    char counter = 0;
    for(char i = 15; i > 7; i--)
    {
        tlc_send_data(tlc_servo[i - counter]>>4);
        tlc_send_data(tlc_servo[i - counter]<<4);
        tlc_send_data(tlc_servo[i - (counter + 1)]);
        counter++;
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat_pin = 1;
    tlc_delay_us(1); 
    tlc_xlat_pin = 0;
    tlc_delay_us(1);

    //T2CONbits.TMR2ON = 1; //Turn timer back on
}

/**************************************************************************
Macro       : tlc_set

Description : Sets the position on the selected servo.

Arguments   : char tlc_servo_number, char value

Remarks     : This function ONLY updates the selected servo position variable,
 it does NOT update the tlc5940.
***************************************************************************/
void tlc_set(char tlc_servo_number, int value) //Value between 0 and 180
{
    if(value == -1 || value < -1 || value > 180)
    {
        tlc_servo[tlc_servo_number] = 0;
        tlc_servo_temp[tlc_servo_number] = 0;
    }
    else
    {
        tlc_servo[tlc_servo_number] = value + 55;
        tlc_servo_temp[tlc_servo_number] = value + 55;
    }
}

/**************************************************************************
Macro       : tlc_write
 
Description : Sets the position on the selected servo then updates the tlc.
 
Arguments   : char tlc_servo_number, char value
 
Remarks     : This is different from tlc_set. This function sets the servo
 position and then automatically updates the tlc with the new information.
***************************************************************************/
void tlc_write(char tlc_servo_number, int value)
{
    if(value == -1 || value < -1 || value > 180)
    {
        tlc_servo[tlc_servo_number] = 0;
        tlc_servo_temp[tlc_servo_number] = 0;
    }
    else
    {
        tlc_servo[tlc_servo_number] = value + 55;
        tlc_servo_temp[tlc_servo_number] = value + 55;
    }

    //Update the tlc grayscale
    //tlc_delay_ms(2);
    tlc_delay_us(1);

    //T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg_pin = 0; //Grayscale input mode
    tlc_xlat_pin = 0;
    tlc_blank_pin = 0;

    char counter = 0;
    for(char i = 15; i > 7; i--)
    {
        tlc_send_data(tlc_servo[i - counter]>>4);
        tlc_send_data(tlc_servo[i - counter]<<4);
        tlc_send_data(tlc_servo[i - (counter + 1)]);
        counter++;
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat_pin = 1;
    tlc_delay_us(1);
    tlc_xlat_pin = 0;
    tlc_delay_us(1);
    
    //T2CONbits.TMR2ON = 1; //Turn timer back on
    //end
}

//Nasty temp delay function until I have a better solution
void tlc_sweep_delay(int count)
{
    while(count != 0)
    {
        tlc_delay_ms(1);
        count--;
    }
}

void tlc_sweep_set(char tlc_servo_number, int value) //Value between 0 and 180
{
    if(value == -1 || value < -1 || value > 180)
    {
        tlc_servo_temp[tlc_servo_number] = 0;
    }
    else
    {
        tlc_servo_temp[tlc_servo_number] = value + 55;
    }
}

void tlc_sweep_update(int speed) //1 slow, 2 medium, 3 fast
{
    //Nasty temp if statements for variable delay until better solution
    if(speed <= 1)
    {
        speed = 100;
    }
    else if(speed == 2)
    {
        speed = 50;
    }
    else if(speed >= 3)
    {
        speed = 10;
    }

    int j = 0;
    while(j != 16) //Means all servo positions are equal to temp positions.
    {
        //Add checks to make sure value is not over 180 or below 0.
        j = 0;
        for(int i = 0; i < 16; i++)
        {
            if(tlc_servo_temp[i] != tlc_servo[i])
            {
                if(tlc_servo_temp[i] > tlc_servo[i]) //Greater than
                {
                    tlc_servo[i] = tlc_servo[i] + 5;
                }
                else //Must be less than
                {
                    tlc_servo[i] = tlc_servo[i] - 5;
                }
            }
            else //Means its equal
            {
                j++;
            }
        }

        //Update the tlc grayscale
        tlc_delay_us(1);

        //T2CONbits.TMR2ON = 0; //Turn off timer
        tlc_vprg_pin = 0; //Grayscale input mode
        tlc_xlat_pin = 0;
        tlc_blank_pin = 0;

        char counter = 0;
        for(char i = 15; i > 7; i--)
        {
            tlc_send_data(tlc_servo[i - counter]>>4);
            tlc_send_data(tlc_servo[i - counter]<<4);
            tlc_send_data(tlc_servo[i - (counter + 1)]);
            counter++;
        }

        //Testing faster toggle speeds
        tlc_delay_us(1);
        tlc_xlat_pin = 1;
        tlc_delay_us(1);
        tlc_xlat_pin = 0;
        tlc_delay_us(1);

        //T2CONbits.TMR2ON = 1; //Turn timer back on
        tlc_sweep_delay(speed);
    }
}

/*
 TODO!!

 Need to make increments of 2 to work instead of just 5, then add different
 speeds.
 */
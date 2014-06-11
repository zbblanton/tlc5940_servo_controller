#include "tlc5940.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

char tlc_servo[16];

void serial_send_data(char data)
{
    SSPBUF = data; //Loads data into SSPBUF reg
    tlc_delay_ms(1);
}

void tlc_init()
{
    //Initialize dot correction
    tlc_vprg = 1; //Dot correction input mode
    tlc_xlat = 0;
    tlc_delay_ms(2);

    serial_send_data(0b11111111);//tlc_servo 0
    serial_send_data(0b11111111);//tlc_servo 1
    serial_send_data(0b11111111);//tlc_servo 2
    serial_send_data(0b11111111);//tlc_servo 3
    serial_send_data(0b11111111);//tlc_servo 4
    serial_send_data(0b11111111);//tlc_servo 5
    serial_send_data(0b11111111);//tlc_servo 6
    serial_send_data(0b11111111);//tlc_servo 7
    serial_send_data(0b11111111);//tlc_servo 8
    serial_send_data(0b11111111);//tlc_servo 9
    serial_send_data(0b11111111);//tlc_servo 10
    serial_send_data(0b11111111);//tlc_servo 11

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;
    //end

    //Initialize grayscale
    tlc_delay_ms(2);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;

    //Initialize the default servo position
    //Change values to your desired default servo position
    tlc_servo[0] = 235;
    tlc_servo[1] = 0;
    tlc_servo[2] = 0;
    tlc_servo[3] = 0;
    tlc_servo[4] = 0;
    tlc_servo[5] = 0;
    tlc_servo[6] = 0;
    tlc_servo[7] = 0;
    tlc_servo[8] = 0;
    tlc_servo[9] = 0;
    tlc_servo[10] = 0;
    tlc_servo[11] = 0;
    tlc_servo[12] = 0;
    tlc_servo[13] = 0;
    tlc_servo[14] = 0;
    tlc_servo[15] = 235;

    serial_send_data(0b00000000);//tlc_servo 0
    serial_send_data(0b00000000);//tlc_servo 1
    serial_send_data(0b00000000);//tlc_servo 2
    serial_send_data(0b00000000);//tlc_servo 3
    serial_send_data(0b00000000);//tlc_servo 4
    serial_send_data(0b00000000);//tlc_servo 5
    serial_send_data(0b00000000);//tlc_servo 6
    serial_send_data(0b00000000);//tlc_servo 7
    serial_send_data(0b00000000);//tlc_servo 8
    serial_send_data(0b00000000);//tlc_servo 9
    serial_send_data(0b00000000);//tlc_servo 10
    serial_send_data(0b00000000);//tlc_servo 11
    serial_send_data(0b00000000);//tlc_servo 0
    serial_send_data(0b00000000);//tlc_servo 1
    serial_send_data(0b00000000);//tlc_servo 2
    serial_send_data(0b00000000);//tlc_servo 3
    serial_send_data(0b00000000);//tlc_servo 4
    serial_send_data(0b00000000);//tlc_servo 5
    serial_send_data(0b00000000);//tlc_servo 6
    serial_send_data(0b00000000);//tlc_servo 7
    serial_send_data(0b00000000);//tlc_servo 8
    serial_send_data(0b00000000);//tlc_servo 9
    serial_send_data(0b00000000);//tlc_servo 10
    serial_send_data(0b00000000);//tlc_servo 11

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;

    serial_send_data(0b10000000);//one clock pulse for first grayscale
    //end
}

void tlc_update()
{   
    tlc_delay_ms(2);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;

    char counter = 0;
    for(char i = 0; i < 8; i++)
    {
        serial_send_data(tlc_servo[i + counter]>>4);
        serial_send_data(tlc_servo[i + counter]<<4);
        serial_send_data(tlc_servo[i + (counter + 1)]);
        counter++;
    }

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;
    T2CONbits.TMR2ON = 1; //Turn timer back on
}

/**************************************************************************
Macro       : tlc_set

Description : Sets the position on the selected servo.

Arguments   : char tlc_servo_number, char value

Remarks     : This function ONLY updates the selected servo position variable,
 it does NOT update the tlc5940.
***************************************************************************/
void tlc_set(char tlc_servo_number, char value) //Value between 0 and 180
{
    tlc_servo[tlc_servo_number] = value + 55;
}

/**************************************************************************
Macro       : tlc_write
 
Description : Sets the position on the selected servo then updates the tlc.
 
Arguments   : char tlc_servo_number, char value
 
Remarks     : This is different from tlc_set. This function sets the servo
 position and then automatically updates the tlc with the new information.
***************************************************************************/
void tlc_write(char tlc_servo_number, char value)
{
    tlc_servo[tlc_servo_number] = value + 55; //Set the servo

    //Update the tlc grayscale
    tlc_delay_ms(2);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;

    char counter = 0;
    for(char i = 0; i < 8; i++)
    {
        serial_send_data(tlc_servo[i + counter]>>4);
        serial_send_data(tlc_servo[i + counter]<<4);
        serial_send_data(tlc_servo[i + (counter + 1)]);
        counter++;
    }

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;
    T2CONbits.TMR2ON = 1; //Turn timer back on
    //end
}
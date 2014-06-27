/*
 * File:   tlc5940.c
 * Author: Zachary Blanton (zbblanton@gmail.com)
 *
 * Created on December 17, 2013, 11:27 PM
 */

#include "tlc5940.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

char tlc_servo[16];
char tlc_servo_temp[16];


void serial_send_data(char data)
{
    SSPBUF = data; //Loads data into SSPBUF reg
    //tlc_delay_ms(1);
    tlc_delay_us(1); //Testing a much faster delay
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
    //Initialize dot correction
    tlc_vprg = 1; //Dot correction input mode
    tlc_xlat = 0;
    tlc_delay_ms(2);

    //Send initial dot correction data
    for(char i = 0; i < 12; i++)
    {
        tlc_send_data(0b11111111);
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat = 1;
    tlc_delay_us(1);
    tlc_xlat = 0;
    tlc_delay_us(1);
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

    //Send initial grayscale data
    for(char i = 0; i < 24; i++)
    {
        tlc_send_data(0b00000000);
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat = 1;
    tlc_delay_us(1);
    tlc_xlat = 0;
    tlc_delay_us(1);

    tlc_send_data(0b10000000);//one clock pulse for first grayscale
    //end
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

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;

    char counter = 0;
    for(char i = 0; i < 8; i++)
    {
        tlc_send_data(tlc_servo[i + counter]>>4);
        tlc_send_data(tlc_servo[i + counter]<<4);
        tlc_send_data(tlc_servo[i + (counter + 1)]);
        counter++;
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat = 1;
    tlc_delay_us(1); 
    tlc_xlat = 0;
    tlc_delay_us(1);

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
    //tlc_servo_temp[tlc_servo_number] = value + 55;
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
    //tlc_delay_ms(2);
    tlc_delay_us(1);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;

    char counter = 0;
    for(char i = 0; i < 8; i++)
    {
        tlc_send_data(tlc_servo[i + counter]>>4);
        tlc_send_data(tlc_servo[i + counter]<<4);
        tlc_send_data(tlc_servo[i + (counter + 1)]);
        counter++;
    }

    //Testing faster toggle speeds
    tlc_delay_us(1);
    tlc_xlat = 1;
    tlc_delay_us(1);
    tlc_xlat = 0;
    tlc_delay_us(1);
    
    T2CONbits.TMR2ON = 1; //Turn timer back on
    //end
}

void tlc_sweep(char num_of_increments)
{
    //NEED TO GO CHANGE FUNCTIONS TO REFLECT TEMP SERVO POS INSTEAD.

   //tlc_servo[tlc_servo_number] = value + 55; //Set the servo

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
                    tlc_servo[i] = tlc_servo[i] + num_of_increments;
                }
                else //Must be less than
                {
                    tlc_servo[i] = tlc_servo[i] - num_of_increments;
                }
            }
            else //Means its equal
            {
                j++;
            }
        }

        //Update the tlc grayscale
        tlc_delay_us(1);

        T2CONbits.TMR2ON = 0; //Turn off timer
        tlc_vprg = 0; //Grayscale input mode
        tlc_xlat = 0;
        tlc_blank = 0;

        char counter = 0;
        for(char i = 0; i < 8; i++)
        {
            tlc_send_data(tlc_servo[i + counter]>>4);
            tlc_send_data(tlc_servo[i + counter]<<4);
            tlc_send_data(tlc_servo[i + (counter + 1)]);
            counter++;
        }

        //Testing faster toggle speeds
        tlc_delay_us(1);
        tlc_xlat = 1;
        tlc_delay_us(1);
        tlc_xlat = 0;
        tlc_delay_us(1);

        T2CONbits.TMR2ON = 1; //Turn timer back on
    }
}

/*
 Use for loop above as a all in one loop.
 Use loop to update all channals at once, depending on which i count it is on
 possibly make a global temp_tlc_servo so that we dont have to reintiaze it
 on every call of the funciton. Have an if statement in the for loop to check
 if new value is different from old, if so then set new value.
 
 Need to add for loop inside of tlc_init to update grayscale data with default
 user positions.
 */
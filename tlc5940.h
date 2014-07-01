/* 
 * File:   tlc5940.h
 * Author: Zachary Blanton (zbblanton@gmail.com)
 *
 * Created on December 17, 2013, 11:27 PM
 */

#ifndef TLC5940_H
#define	TLC5940_H

#include <xc.h>

//Custom pin names.
//Change the assignment pins to the pins you use.
#define tlc_xlat_pin LATDbits.LD0
#define tlc_vprg_pin LATDbits.LD1
#define tlc_blank_pin LATDbits.LD2

//Also be sure to change the TRIS bits for the same pins.
#define tlc_xlat_io TRISDbits.RD0
#define tlc_vprg_io TRISDbits.RD1
#define tlc_blank_io TRISDbits.RD2

//Custom delay function name.
//Change to whatever your delay function name is.
//For example my function is __delay_ms, so change to match your delay function.
#define tlc_delay_ms __delay_ms
#define tlc_delay_us __delay_us

//Custom serial functions name.
//Change to whatever your serial write function name is.
#define tlc_spi_output_pin TRISCbits.RC5
#define tlc_spi_clock_pin TRISCbits.RC3
#define tlc_send_data tlc_spi_send_data

//Custom Timer 2 pin names
#define tlc_pwm_io TRISCbits.RC2

char tlc_servo[16]; //Holds current servo position
char tlc_servo_temp[16]; //Holds new servo position
int tlc_interrupt_counter; //Counter for the interupt

void tlc_spi_init();
void tlc_spi_send_data(char data);
void tlc_pwm_init();
void tlc_init();
void tlc_update();
void tlc_set(char channel_number, char value);
void tlc_write(char channel_number, char value);

//Version 1.1 functions in development
void tlc_sweep_set(char channel_number, char value);
void tlc_sweep_update(char num_of_increments);

//Possible future functions
//void tlc_start();
//void tlc_stop();

#endif	/* TLC5940_H */


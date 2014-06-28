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
#define tlc_xlat LATDbits.LD0
#define tlc_vprg LATDbits.LD1
#define tlc_blank LATDbits.LD2

//Custom delay function name.
//Change to whatever your delay function name is.
//For example my function is __delay_ms, so change to match your delay function.
#define tlc_delay_ms __delay_ms
#define tlc_delay_us __delay_us

//Custom serial write function name.
//Change to whatever your serial write function name is.
#define tlc_send_data serial_send_data

char tlc_servo[16];
char tlc_servo_temp[16];

void serial_init();
void serial_send_data(char data);
void tlc_init();
void tlc_update();
void tlc_set(char channel_number, char value);
void tlc_write(char channel_number, char value);

//Version 1.1 functions in development
void tlc_sweep(char num_of_increments);

//Future functions
//void tlc_sweep(char channel_number, char sweep_type, int num_of_increment);
//void tlc_start();
//void tlc_stop();

#endif	/* TLC5940_H */


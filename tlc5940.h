/* 
 * File:   tlc5940.h
 * Author: Zachary
 *
 * Created on December 17, 2013, 11:27 PM
 */

#ifndef TLC5940_H
#define	TLC5940_H

#include <xc.h>

#define tlc_xlat LATDbits.LD0
#define tlc_vprg LATDbits.LD1
#define tlc_blank LATDbits.LD2

void serial_init();
void serial_send_data(char data);
void test_dot_correction();
void test_grayscale();
void init_dot_correction();
void init_grayscale();
void set_grayscale();
void update_grayscale();
void set_channel(char channel_number, char value);

#endif	/* TLC5940_H */

#include "tlc5940.h"
#include <stdint.h>

#define _XTAL_FREQ 8000000
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

char channel[16];

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

    serial_send_data(0b11111111);//Channel 0
    serial_send_data(0b11111111);//Channel 1
    serial_send_data(0b11111111);//Channel 2
    serial_send_data(0b11111111);//Channel 3
    serial_send_data(0b11111111);//Channel 4
    serial_send_data(0b11111111);//Channel 5
    serial_send_data(0b11111111);//Channel 6
    serial_send_data(0b11111111);//Channel 7
    serial_send_data(0b11111111);//Channel 8
    serial_send_data(0b11111111);//Channel 9
    serial_send_data(0b11111111);//Channel 10
    serial_send_data(0b11111111);//Channel 11

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

    channel[0] = 235;
    channel[1] = 0;
    channel[2] = 0;
    channel[3] = 0;
    channel[4] = 0;
    channel[5] = 0;
    channel[6] = 0;
    channel[7] = 0;
    channel[8] = 0;
    channel[9] = 0;
    channel[10] = 0;
    channel[11] = 0;
    channel[12] = 0;
    channel[13] = 0;
    channel[14] = 0;
    channel[15] = 235;


    serial_send_data(0b00000000);//Channel 0
    serial_send_data(0b00000000);//Channel 1
    serial_send_data(0b00000000);//Channel 2
    serial_send_data(0b00000000);//Channel 3
    serial_send_data(0b00000000);//Channel 4
    serial_send_data(0b00000000);//Channel 5
    serial_send_data(0b00000000);//Channel 6
    serial_send_data(0b00000000);//Channel 7
    serial_send_data(0b00000000);//Channel 8
    serial_send_data(0b00000000);//Channel 9
    serial_send_data(0b00000000);//Channel 10
    serial_send_data(0b00000000);//Channel 11
    serial_send_data(0b00000000);//Channel 0
    serial_send_data(0b00000000);//Channel 1
    serial_send_data(0b00000000);//Channel 2
    serial_send_data(0b00000000);//Channel 3
    serial_send_data(0b00000000);//Channel 4
    serial_send_data(0b00000000);//Channel 5
    serial_send_data(0b00000000);//Channel 6
    serial_send_data(0b00000000);//Channel 7
    serial_send_data(0b00000000);//Channel 8
    serial_send_data(0b00000000);//Channel 9
    serial_send_data(0b00000000);//Channel 10
    serial_send_data(0b00000000);//Channel 11

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;

    serial_send_data(0b10000000);//one clock pulse for first grayscale
    //end
}

void init_dot_correction()
{
    tlc_vprg = 1; //Dot correction input mode
    tlc_xlat = 0;
    tlc_delay_ms(2);

    serial_send_data(0b11111111);//Channel 0
    serial_send_data(0b11111111);//Channel 1
    serial_send_data(0b11111111);//Channel 2
    serial_send_data(0b11111111);//Channel 3
    serial_send_data(0b11111111);//Channel 4
    serial_send_data(0b11111111);//Channel 5
    serial_send_data(0b11111111);//Channel 6
    serial_send_data(0b11111111);//Channel 7
    serial_send_data(0b11111111);//Channel 8
    serial_send_data(0b11111111);//Channel 9
    serial_send_data(0b11111111);//Channel 10
    serial_send_data(0b11111111);//Channel 11

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;
}

void init_grayscale()
{
    tlc_delay_ms(2);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;
    
    channel[0] = 235;
    channel[1] = 0;
    channel[2] = 0;
    channel[3] = 0;
    channel[4] = 0;
    channel[5] = 0;
    channel[6] = 0;
    channel[7] = 0;
    channel[8] = 0;
    channel[9] = 0;
    channel[10] = 0;
    channel[11] = 0;
    channel[12] = 0;
    channel[13] = 0;
    channel[14] = 0;
    channel[15] = 235;


    serial_send_data(0b00000000);//Channel 0
    serial_send_data(0b00000000);//Channel 1
    serial_send_data(0b00000000);//Channel 2
    serial_send_data(0b00000000);//Channel 3
    serial_send_data(0b00000000);//Channel 4
    serial_send_data(0b00000000);//Channel 5
    serial_send_data(0b00000000);//Channel 6
    serial_send_data(0b00000000);//Channel 7
    serial_send_data(0b00000000);//Channel 8
    serial_send_data(0b00000000);//Channel 9
    serial_send_data(0b00000000);//Channel 10
    serial_send_data(0b00000000);//Channel 11
    serial_send_data(0b00000000);//Channel 0
    serial_send_data(0b00000000);//Channel 1
    serial_send_data(0b00000000);//Channel 2
    serial_send_data(0b00000000);//Channel 3
    serial_send_data(0b00000000);//Channel 4
    serial_send_data(0b00000000);//Channel 5
    serial_send_data(0b00000000);//Channel 6
    serial_send_data(0b00000000);//Channel 7
    serial_send_data(0b00000000);//Channel 8
    serial_send_data(0b00000000);//Channel 9
    serial_send_data(0b00000000);//Channel 10
    serial_send_data(0b00000000);//Channel 11
    
    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;

    serial_send_data(0b10000000);//one clock pulse for first grayscale
    
}

void update_grayscale()
{   
    tlc_delay_ms(2);

    T2CONbits.TMR2ON = 0; //Turn off timer
    tlc_vprg = 0; //Grayscale input mode
    tlc_xlat = 0;
    tlc_blank = 0;

    char counter = 0;
    for(char i = 0; i < 8; i++)
    {
        serial_send_data(channel[i + counter]>>4);
        serial_send_data(channel[i + counter]<<4);
        serial_send_data(channel[i + (counter + 1)]);
        counter++;
    }

    tlc_delay_ms(2);
    tlc_xlat = 1;
    tlc_delay_ms(30);
    tlc_xlat = 0;
    T2CONbits.TMR2ON = 1; //Turn timer back on
}

void set_channel(char channel_number, char value) //Value between 0 and 180
{
    channel[channel_number] = value + 55;
}

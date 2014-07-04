# TLC5940 Servo Controller for PIC Microcontrollers

Version 1.0 is ready for download. It still lacks some documentation of the functions, but should work on any of the 
PIC18 series microcontrollers. This version is only for the brave or impatient souls who can't wait for better documentation.
Version 1.1 will have fully documented code and a few great new functions that I think will separate this library from others.

To download the source, go to downloads and click "tlc5940_servo_controller_v1.0.zip".

I should be able to release version 1.1 by the end of this week, so around 7/5/2014. This verison should be fully documented
and I will then be able to release a full tutorial on how to wire the TLC5940 up and how to use the functions.

This code is developed and tested on the PIC18F4520.

![](http://i.imgur.com/F2dGZP9.png)

All the legal stuff can be found in the datasheet:
[http://www.ti.com/lit/ds/symlink/tlc5940.pdf](http://www.ti.com/lit/ds/symlink/tlc5940.pdf)

#Tutorial

Here is what I'm going to call the "cost". This is the requirements thats it going to take to embed this into your project.
Cost:
- 3 general I/O pins.
- The use of timer 2.
- 1 PWM pin.
- Use of SPI so 2 more pins, three if you need to do Chip Selects. (This isn't on the TLC5940 but i'll try to explain this later)

Initial setup before using the TLC5940. 
---

The TLC5940 needs three control pins. By default they are pins 0, 1, and 2 on PORTD. You can change this by editing these lines below located in the tlc5940.h file. 
```
#define tlc_xlat_pin LATDbits.LD0
#define tlc_vprg_pin LATDbits.LD1
#define tlc_blank_pin LATDbits.LD2

#define tlc_xlat_io TRISDbits.RD0
#define tlc_vprg_io TRISDbits.RD1
#define tlc_blank_io TRISDbits.RD2
```
Here is the schematic of my current tlc5940 setup. This schematic uses all the default pins in the code.
PICTURE OF SCHEMATIC

We now need to set the initial servo positions of the servos you're going to use. To do this we use the function tlc_set(servo channel number, degree).
For example to set channel 0 servo position to 140 degrees we call the function below:
tlc_set(1, 140);
Do this for as many servos as you have connected if you need them to be on as soon as you turn the tlc5940 on. Note that you don't need to set all the channels that won't be used to 0. This is done automatically with tlc_init. 

Next we need to initialize the tlc5940, to do this we need to run three functions. 
```
tlc_pwm_init();
tlc_spi_init();
tlc_init();
```
So line 1 we initialize the PWM for the Grayscale clock. This sets up our timer 2 to 100,000hz and makes our timer pin and output.
Line 2 initializes the SPI interface, this sets up the required settings for SPI on the TLC5940 and makes sure the SPI output pin and Clock pin
are set to output. Finally line 3 initializes the tlc5940. This does all kinds of things but the important things you need to know is it sets the 
pins selected for use by the tlc5940 to output and sends the initial data. 

After that we should now be ready to use the tlc5940 to control our servos.

Using the servo control functions.
------


WE have 3 different ways to send new positions to our servos. You may use any of the 3 based on what you need to do.

-The first way is to simply write the position to the TLC5940. To do this we simply call the tlc_write(servo channel number, degree).

For example, to set servo 7 to 180 degrees we call:
tlc_write(7, 180);

This will automatically send the information over to the TLC5940 and the servo will move to that position as fast as possible. This function is nice when you simply want to move 1 servo and it doesn't matter how fast it goes to get there. 

-The next way is to use tlc_set(servo channel number, degree) and then tlc_update. We use tlc_set the same way as above when we were setting the initial servo positions. We do this for each servo we want to move. Then we simply call tlc_update and it sends the information over and all the servos will move to there new positions simultaneously. 

For example: 
Say we want to set several servos to move all at once then we do this:
```
tlc_set(0, 180);
tlc_set(4, 60);
tlc_set(6, 150);
tlc_set(7, 10);
tlc_update();
```

So we set all the servos to what we want them to be and then call tlc_update and they will all move at once to there new position at full speed. This is nice when you have several servos to move and you need them to move all at the same time and you don't care how fast they go.

-The last way is to use tlc_sweet_set and tlc_sweet_update. This is almost exactly like tlc_set and tlc_update from above except this does a sweep motion at variable speeds. 

So say we want to set servo 1 and servo 6 to new positions but want them to go slow, then we can use something like:
```
tlc_sweep_set(1, 150);
tlc_sweep_set(6, 10);
tlc_sweep_update(1);
```
Notice that tlc_sweep_update(desired speed) above has a 1 in the parameter, this means that both servos will move at once but will move the new position slowly. We could have also used a 2 or a 3. The options are 1 for slow, 2 for median, and 3 for fast. This set of functions is nice when you need to move several servos at once (or even just one servo) but you want to control how fast they can move to the new position.

To attempt to show some difference here is a youtube video of my robotic arm going through the different functions.
https://www.youtube.com/watch?v=2360H5ts2hE

One last thing is that if you want to turn a servo off then you can use -1 as the servo position. As of now the only supported ways to turn servos off is one at a time with tlc_write or more than one with tlc_set and tlc_update, this doesn't work for the sweep function.  So for example you if you want to turn servo 2 off then you would use this code:
```
tlc_write(2, -1);
```
or this code:
```
tlc_set(2, -1);
tlc_update();
```

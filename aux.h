
// --------------------------------- //
//                                   //
//    DECLARATIONS IN HEADER FILE    //
//                                   //
// --------------------------------- //

#include <avr/io.h>
#include <util/delay.h>

#ifndef AUX_H
#define AUX_H

// uart things
#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUFF 256

// communication
#define MAX_BUFF 256

// trig on pin12 & echo on pin11
#define TRIG_BIT 6
#define ECHO_BIT 5
#define SENSOR_DDR DDRB
#define SENSOR_PORT PORTB

// servo on pin 13
#define SERVO_BIT 7
#define SERVO_DDR DDRB
#define SERVO_PORT PORTB

// function declarations
void UART_init();

#endif
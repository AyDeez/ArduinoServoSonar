
// --------------------------------- //
//                                   //
//    DECLARATIONS IN HEADER FILE    //
//                                   //
// --------------------------------- //

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef AUX_H
#define AUX_H

// uart things
#define BAUD 19200
#define F_CPU 16000000UL
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUFF 256

// communication
#define MAX_BUFF 256

// trig on pin12 & echo on pin11
#define TRIG_BIT 6
#define ECHO_BIT 5
#define SENSOR_DDR DDRB
#define SENSOR_PORT PORTB

// servo on pin 10
#define SERVO_BIT 4
#define SERVO_DDR DDRB
#define SERVO_PORT PORTB
#define SERVO_PIN PINB

// servo
#define DEFAULT_MIN_ANGLE 0
#define DEFAULT_MAX_ANGLE 180
#define DEFAULT_SAMPLING_ANGLE 10
#define DEFAULT_LOCK_ORIENTATION false

// timers
#define DEFAULT_SAMPLING_FREQ 500

// debug
#define SHOW_ON_SERIAL false

// function declarations
void usage();
void UART_init();
void timer_init(uint8_t time);
void servo_init();
void sensor_init();
void UART_putChar(uint8_t c);
void UART_putString(uint8_t* buf);
uint8_t UART_getChar();
uint8_t UART_getString(uint8_t* buf);
void set_servo_angle(uint8_t angle);
uint8_t calculate_distance();

#endif


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

// communication
#define MAX_BUFF 256

// trig(green) on pin12 & echo(yellow) on pin11
#define TRIG_BIT 6
#define ECHO_BIT 5
#define SENSOR_DDR DDRB
#define SENSOR_PORT PORTB
#define SENSOR_PIN PINB

// servo on pin 5
#define SERVO_BIT 3
#define SERVO_DDR DDRE
#define SERVO_PORT PORTE
#define SERVO_PIN PINE

// servo
#define DEFAULT_MIN_ANGLE 0
#define DEFAULT_MAX_ANGLE 180
#define DEFAULT_SAMPLING_ANGLE 10
#define DEFAULT_LOCK_ORIENTATION true
#define SERVO_FREQ 50

// timers
#define DEFAULT_SAMPLING_FREQ 200

// enums
typedef enum { IDLE, EMITTED, WAITING, READY } sensor_state;

// debug
#define SHOW_ON_SERIAL false

// function declarations
void usage();
void UART_init();
void timer_init(int time);
void servo_init();
void sensor_init();
void UART_putChar(uint8_t c);
void UART_putString(uint8_t* buf);
uint8_t UART_getChar();
uint8_t UART_getString(uint8_t* buf);
void set_servo_angle(uint8_t angle);
uint16_t measure_echo();
void wait_us(uint16_t us);
void wait_ms(uint16_t ms);

#endif

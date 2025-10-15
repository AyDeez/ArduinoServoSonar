
// --------------------------------- //
//                                   //
//    DECLARATIONS IN HEADER FILE    //
//                                   //
// --------------------------------- //

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#ifndef AUX_H
#define AUX_H

// bool
#define TRUE 1
#define FALSE 0

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

// servo
#define DEFAULT_MIN_ANGLE 0
#define DEFAULT_MAX_ANGLE 180

// timers
#define DEFAULT_SAMPLING FREQ 500

// function declarations
void UART_init();
void UART_putChar(uint8_t c);
void UART_putString(uint8_t* buf);
uint8_t UART_getChar();
uint8_t UART_getString(uint8_t* buf);

#endif

#include <avr/io.h>

// --------------------------------- //
//                                   //
//    DECLARATIONS IN HEADER FILE    //
//                                   //
// --------------------------------- //


// BASICS
#define TRUE 1
#define FALSE 0

// DEFAULTS
#define DEFAULT_SAMPLING_FREQ 500

// PINOUT
#define trigPin 37
#define trigBit 0
#define echoPin 36
#define echoBit 1
#define sensorPort PORTC
#define sensorDDR DDRC
#define sensorPin PINC

// TIMER
#define timer5A TCCR5A
#define timer5B TCCR5B
#define timerVal OCR5A
#define timer5Interrupt TIMSK5

// UART THINGS
#define BAUD 19200
#define MYUBRR (F_CPU/16/BAUD-1)
#define MAX_BUFF 256

// COMMANDS
#define SET_SAMPLING_FREQ_CMD "sampling_frequency"
#define SET_MIN_ANGLE_CMD "min_angle"
#define SET_MAX_ANGLE_CMD "max_angle"
#define SET_SAMPLING_ANGLE_CMD "sampling_angle"
#define LOCK_ORIENTATION_CMD "lock_orientation"
#define SHOW_DATA_CMD "show_data"

// STANDARD ENUMS
typedef enum { IN, OUT } pin_direction;
typedef enum { HIGH, LOW, TOGGLE } pin_operation;

// FUNCTION DECLARATIONS
void pin_mode(volatile uint8_t* ddr, uint8_t mask, pin_direction dir);
void pin_operate(volatile uint8_t* port, uint8_t mask, pin_operation op);
void enable_pullup(volatile uint8_t* port, uint8_t mask);

void UART_init();
uint8_t UART_getChar();
uint8_t UART_getString(uint8_t* buf);
void UART_putChar(uint8_t c);
void UART_putString(uint8_t* buf);
void print(const char* msg);

void timer_init(volatile uint8_t* timerA, volatile uint8_t* timerB);
void enable_timer_interrupt(volatile uint8_t* timer_interrupt);
#include <util/delay.h>
#include "aux.h"


// ------------------------------------------- //
//                                             //
//    IMPLEMENTATION OF AUXILIARY FUNCTIONS    //
//                                             //
// ------------------------------------------- //


// sets the pin as input/output
void pin_mode(volatile uint8_t* ddr, uint8_t mask, pin_direction dir) {
    switch (dir) {
        case OUT:
            *ddr |= mask;
            break;
        case IN:
            *ddr &= ~mask;
            break;
    }
}

// sets the port as high/low/toggle
void pin_operate(volatile uint8_t* port, uint8_t mask, pin_operation op) {
    switch (op) {
        case HIGH:
            *port |= mask;
            break;
        case LOW:
            *port &= ~mask;
            break;
        case TOGGLE:
            *port ^= mask;
            break;
    }
}

// enables the pullup resistor on that port
void enable_pullup(volatile uint8_t* port, uint8_t mask) {
    *port |= mask;
}

// inits the UART, sets data to 8bit, enables RX and TX
void UART_init() {
    UBRR0H = (uint8_t) (MYUBRR >> 8);
    UBRR0L = (uint8_t) MYUBRR;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
}

// waits for incoming data, looping on status bit, and returns the received data
uint8_t UART_getChar() {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

// reads a string until the first \n and saves it in the buffer, returns the size read
uint8_t UART_getString(uint8_t* buf) {
    uint8_t* b0 = buf; //beginning of buffer
    while(1){
        uint8_t c = UART_getChar();
        *buf=c;
        buf++;
        // reading a 0 terminates the string
        if (c==0) return buf-b0;
        // reading a \n or a \r return results
        // in forcedly terminating the string
        if(c=='\n'||c=='\r'){
            *buf=0;
            buf++;
            return buf-b0;
        }
    }
}

// waits for trasnmission completed, looping on status bit, then starts transmission
void UART_putChar(uint8_t c) {
    while (!(UCSR0A & (1 << UDRE0)) );
    UDR0 = c;
}

// sends a passed string
void UART_putString(uint8_t* buf) {
    while (*buf) {
        UART_putChar(*buf);
        buf++;
    }
}

// prints a message using UART
void print(const char* msg) {
    UART_putString((uint8_t*)(msg));
}

// inits the timer
void timer_init(volatile uint8_t* timerA, volatile uint8_t* timerB) {
    *timerA = 0;
    *timerB = (1 << WGM52) | (1 << CS50) | (1 << CS52);
    uint16_t ocrval = (uint16_t)(15.625 * DEFAULT_SAMPLING_FREQ);
    timerVal = ocrval;
}

// enables the timer interrupt
void enable_timer_interrupt(volatile uint8_t* timer_interrupt) {
    *timer_interrupt |= (1 << OCIE5A);
}

// ------------------------------------------- //
//                                             //
//    IMPLEMENTATION OF AUXILIARY FUNCTIONS    //
//                                             //
// ------------------------------------------- //

#include "aux.h"

// starts the communication
void UART_init() {
    UBRR0H = (uint8_t) (MYUBRR >> 8); //set baudrate
    UBRR0L = (uint8_t) MYUBRR;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8-bit data
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); //enables RX-TX
}
#include <stdio.h>

#include "avr_common/uart.h"
#include "aux.h"


// TIMER INTERRUPT ROUTINE INSTALLED
ISR(TIMER5_COMPA_vect) {
    //todo
}

// UART INTERRUPT ROUTINE INSTALLED
ISR(USART0_RX_vect) {
    //todo
}


// MAIN
int main() {

    // inits the UART communication
    UART_init();

    // variables
    unsigned int distance;
    

}
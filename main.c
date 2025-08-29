#include <stdio.h>

#include "avr_common/uart.h"
#include "aux.h"


// GLOBAL VARIABLES
uint8_t buf[MAX_BUFF];
volatile int uart_interrupt_occurred = FALSE;
volatile int timer_interrupt_occurred = FALSE;


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

    // inits the UART communication and the timer 5
    UART_init();
    timer_init(&timer5A, &timer5B);    

    // variables
    unsigned int distance;
    char print_string[24];

    // parameters that can be modified
    int min_angle, max_angle;
    int sampling_angle;
    int lock;
    int show_data = FALSE;

    // uart variables
    char* command;
    char* value;

    // mask creation
    const uint8_t trig_mask = (1 << trigBit);
    const uint8_t echo_mask = (1 << echoBit);
    
    // defining input / output and enabling pullup resistors
    pin_mode(&sensorDDR, trig_mask, OUT);
    pin_mode(&sensorDDR, echo_mask, IN);
    enable_pullup(&sensorPort, echo_mask);

    // clearing interrupt flag, enabling timer interrupt and setting interrupt flag
    cli();
    enable_timer_interrupt(timer5Interrupt);
    sei();

    // endless loop
    while (TRUE) {

        break;

    }

}
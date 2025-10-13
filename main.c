#include "aux.h"
#include <avr/io.h>

int main() {

    // enabling communication via UART
    UART_init();

    // global variables
    unsigned long distance;

    // creating mask
    const uint8_t trig_mask = (1 << TRIG_BIT);
    const uint8_t echo_mask = (1 << ECHO_BIT);
    const uint8_t servo_mask = (1 << SERVO_BIT);

    // defining input/output
    SENSOR_DDR |= trig_mask;
    SERVO_DDR |= servo_mask;
    SENSOR_DDR &= ~echo_mask;

    // pullup resistor for echo input
    SENSOR_PORT |= echo_mask;

    // while loop
    while (1) {
        
        // move servo
        // calculate distance
        // print on serial

    }

}
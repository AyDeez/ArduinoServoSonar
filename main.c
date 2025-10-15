#include "aux.h"

// global flags
volatile int interrupt_occurred = FALSE;
volatile int timer_occurred = TRUE;

// UART interrupt routine installation
ISR(USART0_RX_vect) {
    interrupt_occurred = TRUE;
}

// main
int main() {

    // enabling communication via UART
    UART_init();

    // global variables
    uint8_t buf[MAX_BUFF];
    unsigned long distance;
    char* cmd;
    char* value;

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

    // clearing interrupt
    cli();

    // setting interrupt flag
    sei();

    // while infinite loop
    while (TRUE) {

        // if a command is received
        if (interrupt_occurred) {

            // gets from UART the string received
            UART_getString(buf);

            // reset the interrupt flag
            interrupt_occurred = FALSE;

            // cleans the string received
            buf[strcspn(buf, "\r\n")] = 0;

            // setting variables based on the command received
            /*
            if (strcmp(buf,)) {
                do something
            }
            */

            // TEST
            if (strcmp(buf, "ON") == 0) {
                UART_putString((uint8_t*)"HELLO WORLD: ON\n");
            } else if (strcmp(buf, "OFF") == 0) {
                UART_putString((uint8_t*)"HELLO WORLD: OFF\n");
            }

        }

    }

}

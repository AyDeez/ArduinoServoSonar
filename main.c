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

    // enabling communication via UART and prints the usage string
    UART_init();
    usage();

    // global variables
    uint8_t buf[MAX_BUFF];
    unsigned long distance;
    char* cmd;
    char* value;

    // values that can be modified through UART
    uint8_t sampling_freq = DEFAULT_SAMPLING_FREQ;
    uint8_t min_angle = DEFAULT_MIN_ANGLE;
    uint8_t max_angle = DEFAULT_MAX_ANGLE;
    uint8_t sampling_angle = DEFAULT_SAMPLING_ANGLE;
    int lock_orientation = DEFAULT_LOCK_ORIENTATION;
    int show = SHOW_ON_SERIAL;


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


        /*
        TOdo:
            - move servo
            - calculate distance
            - send on serial the data
        */


        // if a command is received, adjust the settings
        if (interrupt_occurred) {

            // gets from UART the string received
            UART_getString(buf);

            // reset the interrupt flag
            interrupt_occurred = FALSE;

            // cleans the string received
            buf[strcspn(buf, "\r\n")] = 0;

            // setting variables based on the command received
            if (strcmp(buf,"usage")==0 || strcmp(buf,"help")==0) {
                usage();
            } else if (strcmp(buf,"show") == 0) {
                show = !show;
                UART_putString("toggled show\n");
            } else if (strcmp(buf,"lock") == 0) {
                lock_orientation = !lock_orientation;
                UART_putString("toggled lock\n");
            }
            
            else {
                UART_putString("command not valid, type 'help' for usage.");
            }

        }

    }

}

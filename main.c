#include "aux.h"

// global flags
volatile bool interrupt_occurred = false;
volatile bool timer_occurred = true;

// UART interrupt routine installation
ISR(USART0_RX_vect) {
    interrupt_occurred = true;
}

// timer interrupt routine installation
ISR(TIMER5_COMPA_vect) {
    timer_occurred = true;
}

// main
int main() {

    // initial setups
    UART_init();                        // enables communication via UART
    timer_init(DEFAULT_SAMPLING_FREQ);  // inits the timer for controlling the while loop
    servo_init();                       // sets the servo as output
    sensor_init();                      // sets the sensor as input/output
    usage();                            // prints the usage on the serial

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
    bool lock_orientation = DEFAULT_LOCK_ORIENTATION;
    bool show = SHOW_ON_SERIAL;

    // clearing interrupt
    cli();

    // enabling the timer int
    TIMSK5 |= (1 << OCIE5A);

    // setting interrupt flag
    sei();

    // servo set at default position
    set_servo_angle(min_angle);
    int current_angle = min_angle;

    // DEBUG
    int i=0;

    // while infinite loop
    while (true) {

        // if sampling freq is reached
        if (timer_occurred) {

            // DEBUG FOR THE TIMER
            char res[16];
            sprintf(res, "%d\n", i++);
            UART_putString(res);

            // reset the timer flag
            timer_occurred = false;

            // if lock flag is FALSE, move the servo
            if (!lock_orientation) {

                // if rotation is not complete, go ahead
                if (current_angle+sampling_angle < max_angle) {
                    current_angle += sampling_angle;
                }

                // else invert and go backwards
                else {
                    sampling_angle = -sampling_angle;
                    current_angle += sampling_angle;
                }

                // move the servo
                set_servo_angle(current_angle);
            }

            // obtain the distance from the sensor
            distance = calculate_distance();

            // finally, if show flag is TRUE, print on serial the distance
            if (show) {
                char res[16];
                sprintf(res, "%lu\n", distance);
                UART_putString(res);
            }
        }

        // if a command is received, adjust the settings
        if (interrupt_occurred) {

            // gets from UART the string received
            UART_getString(buf);

            // reset the interrupt flag
            interrupt_occurred = false;

            // cleans the string received
            buf[strcspn(buf, "\r\n")] = 0;
            cmd = strtok((char*)buf, " ");
            value = strtok(NULL, " ");

            // setting variables based on the command received
            if (strcmp(cmd,"usage")==0 || strcmp(cmd,"help")==0) {
                usage();
            } else if (strcmp(cmd,"show") == 0) {
                show = !show;
                if (show) {
                    UART_putString("toggled show, now set TRUE\n");
                } else {
                    UART_putString("toggled show, now set FALSE\n");
                }
            } else if (strcmp(cmd,"lock") == 0) {
                lock_orientation = !lock_orientation;
                if (lock_orientation) {
                    UART_putString("toggled lock, now set TRUE\n");
                } else {
                    UART_putString("toggled lock, now set FALSE\n");
                }
            } else if (strcmp(cmd, "sampling_frequency")==0 && value!=NULL) {
                uint16_t ms = atoi(value);
                if (ms>=10 && ms<=3000) {
                    OCR5A = (uint16_t)(15.625 * ms);
                    UART_putString("updated sampling frequency\n");
                } else {
                    UART_putString("invalid range (10-3000 ms)\n");
                }
            } else if (strcmp(cmd, "min_angle")==0 && value!=NULL) {
                uint16_t angle = atoi(value);
                if (angle>=DEFAULT_MIN_ANGLE && angle<=DEFAULT_MAX_ANGLE && angle<max_angle) {
                    min_angle = angle;
                    UART_putString("updated min angle\n");
                } else {
                    UART_putString("invalid min angle value\n");
                }
            } else if (strcmp(cmd, "max_angle")==0 && value!=NULL) {
                uint16_t angle = atoi(value);
                if (angle>=DEFAULT_MIN_ANGLE && angle<=DEFAULT_MAX_ANGLE && angle>min_angle) {
                    max_angle = angle;
                    UART_putString("updated max angle\n");
                } else {
                    UART_putString("invalid max angle value\n");
                }
            } else if (strcmp(cmd, "sampling_angle")==0 && value!=NULL) {
                uint16_t angle = atoi(value);
                if (angle>0 && angle<30) {
                    sampling_angle = angle;
                    UART_putString("updated sampling angle value\n");
                } else {
                    UART_putString("invalid sampling angle value\n");
                }
            }
            
            
            
            else {
                UART_putString("command not valid, type 'help' for usage.\n");
            }

        }

    }

}

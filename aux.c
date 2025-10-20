
// ------------------------------------------- //
//                                             //
//    IMPLEMENTATION OF AUXILIARY FUNCTIONS    //
//                                             //
// ------------------------------------------- //

#include "aux.h"

// prints the usage
void usage(void) {
    char msg[512];
    sprintf(
        msg,
        "\n"
        "===============================\n"
        "   ARDUINO SERVO-SONAR MENU\n"
        "===============================\n"
        "Available commands:\n"
        "  - show  : toggle data visualization\n"
        "  - lock  : toggle servo orientation lock\n"
        "  - help  : display this help message\n"
        "  - usage : same as 'help'\n"
        "===============================\n\n"
    );

    UART_putString((uint8_t*)msg);
}

// starts the communication
void UART_init() {
    UBRR0H = (uint8_t) (MYUBRR >> 8); //set baudrate
    UBRR0L = (uint8_t) MYUBRR;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); //8-bit data
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); //enables RX-TX
}

// inits the timer for the loop
void timer_init(int time) {
    TCCR5A = 0; // set prescaler to 1024
    TCCR5B = (1 << WGM52) | (1 << CS50) | (1 << CS52);
    int16_t ocrval=(uint16_t)(15.625*time);
    OCR5A = ocrval;
}

// inits the servo and its relative timer
void servo_init() {
    SERVO_DDR |= (1 << SERVO_BIT);
    //OC1A for compare
    
}

// inits the ultrasonic sensor
void sensor_init() {
    SENSOR_DDR |= (1 << TRIG_BIT);
    SENSOR_DDR &= ~(1 << ECHO_BIT);
    SENSOR_PORT |= (1 << ECHO_BIT);
    TCCR1A = 0;
    TCCR1B = (1 << CS11);
}

// waits for transmission completed, looping on status bit, then starts transmission
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

// sets the angle on the servo based on the microseconds
void set_servo_angle(uint8_t angle) {

}

// waits for a fixed amount of time
void wait_us(uint16_t us) {
    TCCR1A = 0;
    TCCR1B = (1 << CS11);  // set prescaler 8, more precise for us
    TCNT1 = 0;  // reset timer

    // calculates the amount of ticks: F_CPU / prescaler / how many us in a second * us
    uint32_t total_ticks = ((uint32_t)F_CPU/8) / 1000000UL * us;

    while (TCNT1 < total_ticks); // waits for the ticks to complete
}

void wait_ms(uint16_t ms) {
    TCCR1A = 0;
    TCCR1B = (1 << CS11) | (1 << CS10); // set prescaler 64, able to cover high delays
    TCNT1 = 0; // reset timer

    // calculates the amount of ticks per ms: F_CPU / prescaler / how many ms in a second
    uint16_t ticks_per_ms = F_CPU/64/1000;

    for(uint16_t i = 0; i < ms; i++) {
        TCNT1 = 0; // reset timer
        while(TCNT1 < ticks_per_ms); // while stops the iteration for every ms
    }
}

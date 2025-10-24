# OPERATING SYSTEMS PROJECT

## ARDUINO SERVOMOTOR SONAR

This project is part of an exam that is scheduled to be done during the year 2025.

The files in this repo are listed below:

* AVR folder - folder containing the basics of avr programming
* main.c - main file of the project containing the core implementation
* TERMINAL.c - file used for communication between arduino and client
* Makefile - used for compiling and uploading file to the arduino
* .gitignore - used by git


## PROJECT DESCRIPTION:

Arduino controls a sonar system, guiding its orientation through a servomotor and sending the obtained data on the serial port.

Several parameters can be adjusted through the PC, such as:

* sampling frequency
* minimum and maximum angle
* sampling angle
* option to lock the orientation

To display the output, one may use a library of choice.


## HOW IT WORKS
Arduino is connected to an HC-SR04 ultrasonic sensor mounted on a servo motor.
The pinout can be found in the aux.h file.
The main loop uses Timer5 to repeat itself every 'sampling_frequency' ms via a previusly installed interrupt.
Each iteration of the loop performs the following:
* checks the 'lock_orientation' flag; if true, it skips to the distance measurement; otherwise, it moves the servo by 'sampling_angle' degrees.
* calculates the distance using Timer1 to generate a pulse on the TRIG pin, which is then received by the ECHO.
* distance is calculated using the formula: v=s/t.
* checks the 'show' flag; if true, sends the data via UART.
* continuously monitors whether interrupts have been triggered by string reception via UART, and handles any received strings if an interrupt occurs.

The client side consists of a C program responsible for managing communication with the Arduino.
The TERMINAL.c file performs the following operations:
* enumerates and prints on screen all available devices connected to the PC.
* waits for the user to select one of the displayed devices.
* opens the selected serial device using the 'open' function, which returns a file descriptor for subsequent communication.
* cleans any data left in memory, to ensure clean communication.
* configures the serial port parameters (such as baud rate, parity and flow control) using the 'serial_set_interface_attribs()' function.
* creates two processes using 'fork()': the child process (pid = 0), is responsible for sending data via the 'write()' function, while the parent process handles the reception of incoming data.
* the parent process first creates a file named 'output.log' to store all received data. It then performs a second 'fork()' to launch an 'xterm' terminal for-real time data display. The serial file descriptor is comverted into a 'FILE*' stream for easy reading. The program evaluates each received string: if it is a LOG message, it is printed on the main terminal; otherwise it is displayed in the 'xterm' terminal.
* when the string "exit" is sent from the terminal, both child and parent processes terminate.
* finally, all opened files, streams and file descriptors are properly closed and released from memory.



- started on 13/10/2025
- finished on 24/10/2025
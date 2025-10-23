#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/select.h>

int serial_set_interface_attribs(int fd, int speed, int parity);
bool is_log(char* s);

int main() {

    // show available devices
    printf("available devices:\n");
    system("ls /dev/ttyACM* 2>/dev/null");

    // user enters device name, limiting to 63 characters
    char device_name[64];
    printf("enter the device name: ");
    scanf("%63s", device_name);

    // opening the serial
    int serial_fd = open(device_name, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd == -1) {
        printf("error %d opening serial, fd %d\n", errno, serial_fd);
        return -1;
    }

    // cleaning everything left in memory
    tcflush(serial_fd, TCIOFLUSH);

    // setting up parameters for the serial device
    if (serial_set_interface_attribs(serial_fd, 19200, 0) != 0) {
        printf("error in serial_set_interface_attribs\n");
        return -1;
    }

    // debug
    printf("connection open on %s\n", device_name);

    // creating fork processes
    pid_t pid = fork();
    int status;
    if (pid < 0) {
        printf("error with fork\n");
        return -1;
    }

    // child process for sending data
    if (pid == 0) {
        char msg[128];
        while (1) {
            fgets(msg, sizeof(msg), stdin);
            write(serial_fd, msg, strlen(msg));
            if (strcmp(msg, "exit\n") == 0) break;
        }
        close(serial_fd);
        exit(0);

    // parent process for receiving data
    } else {

        // creating file for output
        FILE* output = fopen("output.log", "w");
        if (!output) {
            printf("error while creating output file\n");
            return -1;
        }

        // opening xterm for showing output
        pid_t p = fork();
        if (p == 0) {
            execlp("xterm", "xterm", "-hold", "-e", "tail", "-f", "output.log", NULL);
            printf("error opening xterm\n");
            return -1;
        }

        // converting from file descriptor to FILE*
        FILE* serial_file = fdopen(serial_fd, "r");
        if (!serial_file) {
            printf("error while converting serial file descriptor to FILE*\n");
            return -1;
        }

        char line[256];
        while (fgets(line, sizeof(line), serial_file)) {

            if (strcmp(line, "exit\n") == 0) break;
            
            // if string received is a log string, print in the main terminal
            if (is_log(line)) {
                printf("[LOG]: %s", line);
            }
            
            // else if it is data from the sensor, save it to a file
            else {
                fprintf(output, line);
                fflush(output);
            }
            fflush(stdout);
        }

        kill(p,SIGTERM);
        fclose(output);
        fclose(serial_file);
        close(serial_fd);
        wait(&status);
    }

    return 0;
}



// serial setters
int serial_set_interface_attribs(int fd, int speed, int parity) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd,&tty) != 0) {
        printf("error %d from tcgetattr\n", errno);
        return -1;
    }

    switch (speed) {
        case 19200:
            speed = B19200;
            break;
        case 115200:
            speed = B115200;
            break;
        default:
            printf("cannot set baudrate %d\n", speed);
            return -1;
    }

    cfsetospeed(&tty,speed);
    cfsetispeed(&tty,speed);
    cfmakeraw(&tty);

    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag |= parity;
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_cflag |= CREAD | CLOCAL;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("error %d from tcsetattr\n", errno);
        return -1;
    }

    return 0;
}

bool is_log(char* s) {
    if (s[0] == '0') {
        return false;
    }
    return true;
}

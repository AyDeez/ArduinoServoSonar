#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>



int serial_set_interface_attribs(int fd, int speed, int parity);



int main() {

    // list available devices
    char devices[128];
    printf("available devices:\n");
    system("ls /dev/ttyACM* 2>/dev/null");

    // user enters the device name
    char device_name[64];
    printf("enter the devide name: ");
    scanf("%63s", device_name);

    // connecting to the serial
    int serial_fd = open(device_name, O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd == -1) {
        printf("error %d opening serial, fd %d\n", errno, serial_fd);
        return 1;
    }

    // setting the device preferences
    if (serial_set_interface_attribs(serial_fd, 19200, 0) != 0) {
        printf("error in serial_set_interface_attribs\n");
        return -1;
    }

    // debug
    printf("connection open on %s\n", device_name);

    // fork for separating processes
    pid_t pid = fork();
    if (pid < 0) {
        printf("error with fork\n");
        return -1;
    }

    // child process for sending data
    if (pid == 0) {
        char msg[128];

        while (1) {

        }

        close(serial_fd);
        exit(0);

    }
    
    // father process for receiving data
    else {

        char buffer[128];

    }

    // return success
    return 0;

}



int serial_set_interface_attribs(int fd, int speed, int parity) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd,&tty) != 0) {
        printf("error %d from tcgetattr", errno);
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

    //enable reading
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag |= parity;
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("error %d from tcsetattr", errno);
        return -1;
    }

    return 0;
}

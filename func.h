#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Linux header*/
#include <fcntl.h>   // Contains file controls like O_RDWR
#include <errno.h>   // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h>  // write(), read(), close()


/* Open port*/
int serial_port;
int check_port(){
    serial_port = open("/dev/ttyUSB0", O_RDWR);
    if (serial_port < 0) {
        printf("Have no port conected!!!\n");
        printf("error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }
}

/* Initialize port*/
struct termios tty;
int port_init(){
    /* Read in existing settings, and handle any error*/
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 1;
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1) 

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_cc[VTIME] = 1;    // In this mode, read() will block until either VMIN characters have been received,
    tty.c_cc[VMIN] = 10;

    /* Set in/out baud rate to be 9600*/
    cfsetispeed(&tty, B19200);
    cfsetospeed(&tty, B19200);

    /* Save tty settings, also checking for error*/
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 1;
    }
}

/* Calculate checksum8 XOR */
unsigned char calculateXORChecksum(const unsigned char *data, int length) {
    unsigned char checksum = 0;

    for (int i = 0; i < length; i++) {
        checksum ^= data[i];
    }

    return checksum;
}
/* Write data to serial*/
void write_data(unsigned char *dataframe, int size, int port) {
    // unsigned char xorChecksum = calculateXORChecksum(dataframe, size);
    // dataframe[size-1] = xorChecksum;

    /* Show hex data that sent to serial*/
    printf("Hex message sent: ");
    for (int i = 0; i < size; i++) {
        printf("0x%02X  ", *(dataframe + i));
    }
    // printf("XOR Checksum: 0x%02X\n", xorChecksum);

    write(port, dataframe, size);
}

/* Print suggestion*/
void print_suggestion() {
    printf("Please select options...\n\r \
    -R: Reset all configuration of the module to factory default setting.\n\r \
    -L: Set all LED ON. \n\r \
    -l: Set all LED OFF. \n\r \
    -A: Turn Automatic Detecting Card mode ON\n\r \
    -a: Turn Automatic Detecting Card mode OFF \n\r \
    -r: Read data from RFID card\n\r \
    -h: Sent custom hex (rfid-testtool -h <Hex code>)\n");
}
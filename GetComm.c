#include "func.h"

int serial_port;

int main(int argc, char *argv[])
{
    if (check_port() == 1 || port_init() == 1) return 1;

    // unsigned char msghex[] = { 0x07, 0x0F, 0x52, 0x45, 0x53, 0x45, 0x54, 0x5D };
    /* Get data from linux console*/
    if(argc-1 == 0){
        printf("No thing sent to serial...\r\n");
        return 1;
    }
    unsigned char msghex[argc];
    memset(&msghex, '\0', sizeof(msghex));
    for (int i = 1; i<argc;i++){
        msghex[i-1] = strtol(argv[i],NULL,16);
    }
    /* Calculate the XOR checksum*/
    unsigned char xorChecksum = calculateXORChecksum(msghex, sizeof(msghex) / sizeof(msghex[0]));
    msghex[argc-1] = xorChecksum;

    // printf("XOR Checksum: 0x%02X\n", xorChecksum);
    write(serial_port, msghex, sizeof(msghex));

    /* Allocate memory for read buffer, set size according to your needs*/
    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));

    /* Read data from serial*/
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
        return 1;
    }
    /* Show hex data that was received from serial*/
    printf("Received hex message: ");
    for (int i = 0; i < num_bytes; i++) {
        printf("0x%02X  ", read_buf[i]);
    }
    printf("\nRead %i bytes. Received message: %s\n", num_bytes, read_buf);

    close(serial_port);
    return 0;
}

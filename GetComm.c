#include "func.h"
#include "option.h"

// int serial_port;

int main(int argc, char *argv[])
{
    if (check_port() == 1 || port_init() == 1) return 1;

    /* Get option from linux console*/
    if(argc-1 == 0){
        print_suggestion();
        return 1;
    }
    else if (argc > 2)
    {
        printf("Too many argument...\n\r");
        return 1;
    }

    /* Write data to serial*/
    if(strcmp(argv[1],"-R")==0){
        write_data(F_reset,sizeof(F_reset)/sizeof(F_reset[0]), serial_port);
    }
    else if (strcmp(argv[1],"-L")==0)
    {
        write_data(All_LED_ON,sizeof(All_LED_ON)/sizeof(All_LED_ON[0]),serial_port);
    }
    else if (strcmp(argv[1],"-l")==0)
    {
        write_data(All_LED_OFF,sizeof(All_LED_OFF)/sizeof(All_LED_OFF[0]),serial_port);
    }
    else if (strcmp(argv[1],"-A")==0)
    {
        write_data(Auto_DT_ON,sizeof(Auto_DT_ON)/sizeof(Auto_DT_ON[0]),serial_port);
    }
    else if (strcmp(argv[1],"-a")==0)
    {
        write_data(Auto_DT_OFF,sizeof(Auto_DT_OFF)/sizeof(Auto_DT_OFF[0]),serial_port);
    }
    else if (strcmp(argv[1],"-r")==0)
    {
        write_data(Read_data,sizeof(Read_data)/sizeof(Read_data[0]),serial_port);
    }
    else if (strcmp(argv[1],"--help")==0)
    {
        print_suggestion();
        return 1;
    }
    else {
        printf("Bad argument!!!\n\r");
        print_suggestion();
        return 1;
    }  

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
    printf("\nRead %i bytes. Hex message: ",num_bytes);
    for (int i = 0; i < num_bytes; i++) {
        printf("0x%02X  ", read_buf[i]);
    }
    printf("\nRead %i bytes. ASCII message: %s\n", num_bytes, read_buf);

    close(serial_port);
    return 0;
}

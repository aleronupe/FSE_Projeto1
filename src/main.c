#include <stdio.h>
#include <unistd.h>  //Used for UART
#include <fcntl.h>   //Used for UART
#include <termios.h> //Used for UART
#include <string.h>
#include "../inc/crc16.h"
#include "../inc/bme280.h"

void monta_options_uart(int uart0_filestream)
{
    struct termios options;
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcgetattr(uart0_filestream, &options);
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
}

int main(int argc, const char *argv[])
{

    ////////////// UART///////////////////////
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
        printf("Erro - Não foi possível iniciar a UART.\n");

    monta_options_uart(uart0_filestream);

    unsigned char msg_TI[20] = {0x01, 0x23, 0xC1, 0x0, 0x8, 0x4, 0x0};
    short crc_TI = calcula_CRC(msg_TI, 7);
    memcpy(&msg_TI[7], (const void *)&crc_TI, 2);

    unsigned char msg_TP[20] = {0x01, 0x23, 0xC2, 0x0, 0x8, 0x4, 0x0};
    short crc_TP = calcula_CRC(msg_TP, 7);
    memcpy(&msg_TP[7], (const void *)&crc_TP, 2);

    if (uart0_filestream != -1)
    {
        int count = write(uart0_filestream, &msg_TI[0], 9);
        if (count < 0)
            printf("UART TX error\n");
        else
            printf("escrito.\n");

        sleep(1);

        unsigned char rx_buffer[256];
        int rx_length = read(uart0_filestream, (void *)rx_buffer, 255);
        float temperatura;
        memcpy(&temperatura, &rx_buffer[3], 4);
        if (rx_length > 0)
        {
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos : [%s]\n", rx_length, rx_buffer);
            unsigned char aux[20];
            short pre_crc, obtained;
            memcpy(&aux[0], (const void *)&rx_buffer[0], 7);
            memcpy(&pre_crc, &rx_buffer[7], 2);
            printf("crc recebido: %d\n", pre_crc);
            obtained = calcula_CRC(aux, 7);
            printf("crc calculado: %d\n", obtained);
        }
    }
    close(uart0_filestream);

    ////////////////////// I2C ///////////////////

    return 0;
}

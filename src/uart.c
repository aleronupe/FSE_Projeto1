#include <stdio.h>
#include <unistd.h>  //Used for UART
#include <fcntl.h>   //Used for UART
#include <termios.h> //Used for UART
#include <string.h>
#include <stdlib.h>
#include "../inc/crc16.h"

void abre_uart(int *uart0_filestream)
{
    *uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (*uart0_filestream == -1){
        printf("Erro - Não foi possível iniciar a UART.\n");
    }
}

void monta_uart(int uart0_filestream)
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

int confere_crc(unsigned char *rx_buffer, int rx_length)
{
    if (rx_length > 0)
    {
        short crc_calc, crc_rec;

        memcpy(&crc_rec, (const void *)(rx_buffer + 7), 2);

        crc_calc = calcula_CRC(rx_buffer, 7);
        if (crc_calc == crc_rec)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

float le_temperatura(int uart0_filestream, unsigned char sub_cod)
{
    float temp;
    unsigned char msg_temp[20] = {0x01, 0x23, sub_cod, 0x0, 0x8, 0x4, 0x0};
    short crc_temp = calcula_CRC(msg_temp, 7);
    memcpy(&msg_temp[7], (const void *)&crc_temp, 2);

    int write_result = write(uart0_filestream, &msg_temp[0], 9);
    if (write_result < 0){
        printf("UART TX error\n");
        exit(0);
    }

    usleep(100000);
    // sleep(1);

    int rx_length = 0, out_crc = 0;
    unsigned char rx_buffer[256];
    int count = 0;
    while (out_crc != 1 && count < 11)
    {
        if(count > 0){
            printf("Erro na leitura dos dados da UART\n");
            printf("Requisitando dados novamente...(%d/10)\n", count);
        }
            
        rx_length = read(uart0_filestream, (void *)rx_buffer, 255);
        out_crc = confere_crc(&rx_buffer[0], rx_length);
        count++;
    }

    memcpy(&temp, &rx_buffer[3], 4);
    return temp;
}

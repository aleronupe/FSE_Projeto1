#include <stdio.h>
#include <unistd.h>  //Used for UART
#include <fcntl.h>   //Used for UART
#include <termios.h> //Used for UART
#include <string.h>
#include <linux/i2c-dev.h> //Used for I2C
#include <sys/ioctl.h>     //Used for I2C
#include <stdlib.h>
#include "../inc/crc16.h"
#include "../inc/bme280.h"
#include "../inc/i2c.h"

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

int main(int argc, const char *argv[])
{

    ////////////// UART///////////////////////
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
        printf("Erro - Não foi possível iniciar a UART.\n");

    monta_uart(uart0_filestream);

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
        float temp_TI;
        memcpy(&temp_TI, &rx_buffer[3], 4);
        if (rx_length > 0)
        {
            short crc_calc, crc_rec;

            memcpy(&crc_rec, &rx_buffer[7], 2);
            printf("crc recebido: %d\n", crc_rec);

            crc_calc = calcula_CRC(rx_buffer, 7);
            printf("crc calculado: %d\n", crc_calc);

            memcpy(&temp_TI, &rx_buffer[3], 4);
            printf("temp_TI: %f\n", temp_TI);
        }
    }
    close(uart0_filestream);

    ////////////////////// I2C ///////////////////

    double temp_ambiente = 0.0;

    struct bme280_dev dev;
    struct identifier id;

    monta_i2c(&dev, &id);
    abre_i2c(&dev, &id);
    le_temp_i2c(&dev, &temp_ambiente);
    printf("temperatura ambiente na main: %lf\n", temp_ambiente);
    printf("Agora com módulos :)");

    close(id.fd);

    return 0;
}


#include <stdio.h>
#include <unistd.h>  //Used for UART
#include <fcntl.h>   //Used for UART
#include <termios.h> //Used for UART
#include <string.h>
#include <linux/i2c-dev.h> //Used for I2C
#include <sys/ioctl.h>     //Used for I2C
#include "../inc/crc16.h"
#include "../inc/bme280.h"

/* Structure that contains identifier details used in example */
struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;

    /* Variable that contains file descriptor */
    int8_t fd;
};

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

/*!
 *  @brief Function for reading the sensor's registers through I2C bus.
 *
 *  @param[in] reg_addr       : Register address.
 *  @param[out] data          : Pointer to the data buffer to store the read data.
 *  @param[in] len            : No of bytes to read.
 *  @param[in, out] intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs.
 *
 *  @return Status of execution
 *
 *  @retval 0 -> Success
 *  @retval > 0 -> Failure Info
 *
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
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
        float temp_TI;
        memcpy(&temp_TI, &rx_buffer[3], 4);
        if (rx_length > 0)
        {
            rx_buffer[rx_length] = '\0';
            printf("%i Bytes lidos : [%s]\n", rx_length, rx_buffer);
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
    struct bme280_dev dev;

    struct identifier id;

    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    return 0;
}

#include <stdio.h>
#include <unistd.h>  //Used for UART
#include <fcntl.h>   //Used for UART
#include <termios.h> //Used for UART
#include <string.h>
#include <linux/i2c-dev.h> //Used for I2C
#include <sys/ioctl.h>     //Used for I2C
#include <stdlib.h>
#include <wiringPi.h> //Used for GPIO
#include <softPwm.h>  //Used for GPIO
#include "../inc/crc16.h"
#include "../inc/bme280.h"
#include "../inc/i2c.h"
#include "../inc/lcd.h"

void abre_uart(int *uart0_filestream)
{
    *uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (*uart0_filestream == -1)
        printf("Erro - Não foi possível iniciar a UART.\n");
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
        printf("crc recebido: %d\n", crc_rec);

        crc_calc = calcula_CRC(rx_buffer, 7);
        printf("crc calculado: %d\n", crc_calc);

        if (crc_calc == crc_rec)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

void le_temperatura(int uart0_filestream, unsigned char sub_cod, float *temp)
{
    unsigned char msg_temp[20] = {0x01, 0x23, sub_cod, 0x0, 0x8, 0x4, 0x0};
    short crc_temp = calcula_CRC(msg_temp, 7);
    memcpy(&msg_temp[7], (const void *)&crc_temp, 2);

    int count = write(uart0_filestream, &msg_temp[0], 9);
    if (count < 0)
        printf("UART TX error\n");

    usleep(100000);
    // sleep(1);

    int rx_length = 0, out_crc = 0;
    unsigned char rx_buffer[256];
    while (out_crc != 1)
    {
        out_crc = confere_crc(&rx_buffer[0], rx_length);
        rx_length = read(uart0_filestream, (void *)rx_buffer, 255);
    }

    memcpy(temp, &rx_buffer[3], 4);
}

int main(int argc, const char *argv[])
{

    ////////////// UART///////////////////////
    int uart0_filestream = -1;
    float temp_int, temp_ref;

    monta_uart(uart0_filestream);
    abre_uart(&uart0_filestream);

    if (uart0_filestream != -1)
    {
        le_temperatura(uart0_filestream, 0xC1, &temp_int);
        le_temperatura(uart0_filestream, 0xC2, &temp_ref);
        printf("Temperatura Interna: %f\n", temp_int);
        printf("Temperatura de Referência: %f\n", temp_ref);
    }
    close(uart0_filestream);

    ////////////////////// I2C ///////////////////

    double temp_amb = 0.0;

    struct bme280_dev dev;
    struct identifier id;
    // int8_t rslt = BME280_OK;

    monta_i2c(&dev, &id);
    abre_i2c(&dev, &id);
    inicializa_bme280_i2c(&dev);
    configura_bme280_i2c(&dev);
    le_temp_bme280_i2c(&dev, &temp_amb);
    printf("temperatura Ambiente na main: %lf\n", temp_amb);
    printf("Agora com módulos :)\n");

    close(id.fd);

    ////////////////////// GPIO ///////////////////

    int PWM_pin_res = 4, PWM_pin_vent = 5; /* GPIO1 as per WiringPi,GPIO18 as per BCM */
    int intensity;
    wiringPiSetup();

    pinMode(PWM_pin_res, OUTPUT); /* set GPIO as output */
    softPwmCreate(PWM_pin_res, 1, 100);

    pinMode(PWM_pin_vent, OUTPUT); /* set GPIO as output */
    softPwmCreate(PWM_pin_vent, 1, 100);

    for (intensity = 0; intensity < 101; intensity = intensity + 10)
    {
        softPwmWrite(PWM_pin_vent, intensity); /* change the value of PWM */
        sleep(1);
    }

    softPwmWrite(PWM_pin_vent, 0);

    for (intensity = 100; intensity >= 0; intensity--)
    {
        softPwmWrite(PWM_pin_res, intensity);
        sleep(1);
    }

    softPwmWrite(PWM_pin_res, 0);

    ////////////////// LCD /////////////////

    lcd_init(); // setup LCD

    int LINE1 = 0x80; // 1st line
    int LINE2 = 0xC0; // 2nd line

    lcdLoc(LINE1);
    typeln("Using wiringPi");
    lcdLoc(LINE2);
    typeln("Alexandre editor.");

    return 0;
}

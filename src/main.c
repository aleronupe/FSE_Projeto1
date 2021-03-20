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
#include "../inc/uart.h"

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

    for (intensity = 100; intensity >= 0; intensity = intensity - 10)
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

#include <stdio.h>
#include <unistd.h>  //Used for UART
#include <fcntl.h>   //Used for UART
#include <termios.h> //Used for UART
#include <string.h>
#include <linux/i2c-dev.h> //Used for I2C
#include <sys/ioctl.h>     //Used for I2C
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "../inc/crc16.h"
#include "../inc/bme280.h"
#include "../inc/i2c.h"
#include "../inc/lcd.h"
#include "../inc/uart.h"
#include "../inc/pid.h"
#include "../inc/csv.h"
#include "../inc/gpio.h"

int flag_faz_controle = 1;

/* Variáveis Globais da UART */
int uart0_filestream = -1;

/* Variáveis Globais do I2C */
struct identifier id;

void fecha_conexoes();

int main(int argc, const char *argv[])
{

    signal(SIGINT, fecha_conexoes);
    signal(SIGKILL, fecha_conexoes);

    /*************** Variáveis ************/
    int flag_grava_csv = 0;

    /* Variáveis da UART */
    float temp_int, temp_ref;

    /* Variáveis do I2C */
    double temp_amb = 0.0;
    struct bme280_dev dev;

    /* Variáveis do PID */
    double Kp = 5.0, Ki = 1.0, Kd = 5.0;
    double sinal_controle = 0;

    /* Variáveis do GPIO */
    int intensity;

    /************ Configuração ************/
    /* Configuração da UART */
    monta_uart(uart0_filestream);
    abre_uart(&uart0_filestream);

    /* Configuração do I2C */
    monta_i2c(&dev, &id);
    abre_i2c(&dev, &id);
    inicializa_bme280_i2c(&dev);
    configura_bme280_i2c(&dev);

    /* Configuração do PID */
    pid_configura_constantes(Kp, Ki, Kd);

    /* Configuração do GPIO */
    configura_GPIO();

    /* Configuração do LCD */
    lcd_init();

    /* CSV */
    abre_ou_cria_csv();

    while (flag_faz_controle)
    {
        /* UART */
        le_temperatura(uart0_filestream, 0xC1, &temp_int);
        le_temperatura(uart0_filestream, 0xC2, &temp_ref);

        /* I2C */
        le_temp_bme280_i2c(&dev, &temp_amb);

        /* LCD */
        escreve_temperaturas_lcd(temp_int, temp_ref, temp_amb);
        printf("temperatura Ambiente: %lf\n", temp_amb);
        printf("Temperatura Interna: %f\n", temp_int);
        printf("Temperatura de Referência: %f\n", temp_ref);

        /* PID */
        pid_atualiza_referencia(temp_ref);
        sinal_controle = pid_controle(temp_int);
        printf("PID: %f\n", sinal_controle);

        /* GPIO */
        intensity = sinal_controle;
        ativa_circuito_de_potencia(intensity);

        if (flag_grava_csv == 0)
            flag_grava_csv = 1;
        else
        {
            unsigned int tempo = (unsigned)time(NULL);
            escreve_csv(tempo, temp_int, temp_amb, temp_ref, sinal_controle);
            printf("Gravou CSV");
            flag_grava_csv = 0;
        }

        printf("------------------------\n");

        usleep(700000);
    }

    /************ Finalização ************/
    fecha_conexoes();

    return 0;
}

void fecha_conexoes()
{
    flag_faz_controle = 0;
    /* Finalização da UART */
    printf("\nFinalizando conexão com UART...\n");
    close(uart0_filestream);
    printf("Finalizado!\n");

    /* Finalização do I2C */
    printf("Finalizando conexão com I2C...\n");
    close(id.fd);
    printf("Finalizado!\n");

    /* Finalização do GPIO */
    printf("Finalizando conexão com GPIO...\n");

    printf("Finalizado!\n");
    desativa_circuito_de_potencia();
    printf("Tchau!\n");
    exit(0);
}

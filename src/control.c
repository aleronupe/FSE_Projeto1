#include "control.h"

int flag_faz_controle = 1;

/* Variáveis Globais da UART */
int uart0_filestream = -1;

/* Variáveis Globais do I2C */
struct identifier id;

void controle_temp(void *args)
{
    /*************** Variáveis ************/

    Arg_Struct *struct_received = (Arg_Struct *)args;

    /* Variáveis do UART */
    float temp_ref;

    /* Variáveis do CSV */
    int flag_grava_csv = 0;

    /* Variáveis do I2C */
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

    while (struct_received->flag_run)
    {
        /* UART */
        struct_received->temp_int = le_temperatura(uart0_filestream, 0xC1);
        struct_received->temp_ref_pot = le_temperatura(uart0_filestream, 0xC2);
        if(struct_received->flag_insert_temp){
            temp_ref = struct_received->temp_ref_user;
        } else {
            temp_ref = struct_received->temp_ref_pot;
        }

        /* I2C */
        struct_received->temp_ext = le_temp_bme280_i2c(&dev);

        /* LCD */
        escreve_temperaturas_lcd(struct_received->temp_int, temp_ref, struct_received->temp_ext);

        /* PID */
        pid_atualiza_referencia(temp_ref);
        sinal_controle = pid_controle(struct_received->temp_int);

        /* GPIO */
        intensity = sinal_controle;
        ativa_circuito_de_potencia(intensity);
        struct_received->intensity = intensity;

        if (flag_grava_csv == 0)
            flag_grava_csv = 1;
        else
        {
            escreve_csv(struct_received->temp_int, struct_received->temp_ext, temp_ref, sinal_controle);
            flag_grava_csv = 0;
        }

        usleep(700000);
    }

    /************ Finalização ************/
    fecha_conexoes();
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
    desativa_circuito_de_potencia();
    printf("Finalizado!\n");
    printf("Tchau!\n");
    exit(0);
}

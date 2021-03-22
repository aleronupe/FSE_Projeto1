#include <wiringPi.h> //Used for GPIO
#include <softPwm.h>  //Used for GPIO
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Variáveis Globais do GPIO */
#define PWM_PIN_RES 4
#define PWM_PIN_VENT 5

void configura_GPIO()
{
    wiringPiSetup();
    pinMode(PWM_PIN_RES, OUTPUT);
    softPwmCreate(PWM_PIN_RES, 1, 100);
    pinMode(PWM_PIN_VENT, OUTPUT);
    softPwmCreate(PWM_PIN_VENT, 1, 100);
}

void ativa_circuito_de_potencia(int intensity)
{
    if (intensity > 0)
    {
        softPwmWrite(PWM_PIN_VENT, 0);
        softPwmWrite(PWM_PIN_RES, abs(intensity));
    }
    else if (intensity <= -40)
    {

        softPwmWrite(PWM_PIN_RES, 0);
        softPwmWrite(PWM_PIN_VENT, abs(intensity));
    }
    else
    {
        softPwmWrite(PWM_PIN_VENT, 0);
        softPwmWrite(PWM_PIN_RES, 0);
    }
}

void desativa_circuito_de_potencia()
{
    softPwmWrite(PWM_PIN_VENT, 0);
    usleep(500000);

    softPwmWrite(PWM_PIN_RES, 0);
    usleep(500000);
}

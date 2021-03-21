#ifndef CONTROL_H_
#define CONTROL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "crc16.h"
#include "bme280.h"
#include "i2c.h"
#include "lcd.h"
#include "uart.h"
#include "pid.h"
#include "csv.h"
#include "gpio.h"

typedef struct arg_struct
{
    int flag_run;
    int flag_insert_temp;
    float temp_ext;
    float temp_int;
    double temp_ref_user;
    double temp_ref_pot;

} Arg_Struct;

void controle_temp();

void fecha_conexoes();

#endif /* CONTROL_H_ */
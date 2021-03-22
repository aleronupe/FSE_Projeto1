#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "control.h"
#include "menu.h"

Arg_Struct main_struct;

void mata_threads()
{
    main_struct.flag_run = 0;
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, mata_threads);
    signal(SIGKILL, mata_threads);

    main_struct.flag_insert_temp = 0;
    main_struct.flag_run = 1;

    pthread_t control_tid;
    pthread_t menu_tid;

    iniciaTelas();

    pthread_create(&control_tid, NULL, (void *)controle_temp, (void *)&main_struct);
    pthread_create(&menu_tid, NULL, (void *)atualiza_temperaturas, (void *)&main_struct);

    opcoes_usuario(&main_struct);

    pthread_join(control_tid, NULL);
    pthread_join(menu_tid, NULL);

    return 0;
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include <ncurses.h>
#include <curses.h>

#include "menu.h"
#include "control.h"

WINDOW *windowImprimeDados;

Arg_Struct main_struct;

void mata_threads()
{
    main_struct.flag_run = 0;
}

void createImprimeDadosWindow(){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int tamXwindow = xMax/10 * 8, posX = 2;
    int tamanhoYwindow = yMax/10 * 6;

    windowImprimeDados = newwin(tamanhoYwindow, tamXwindow, 1, posX);
    
    box(windowImprimeDados, 0, 0);
    wrefresh(windowImprimeDados);
}

void imprimeDados(){   

    while(1){
        
        wclear(windowImprimeDados);
        mvwprintw(windowImprimeDados, 1, 1, "Temperatura Interna = %f\nTemperatura Externa = %lf\nTemperatura de Referência = %f\n",main_struct.temp_int ,main_struct.temp_ext ,main_struct.temp_ref_pot);
        
        wrefresh(windowImprimeDados);
        sleep(1);
    }
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, mata_threads);
    signal(SIGKILL, mata_threads);

    main_struct.flag_insert_temp = 0;
    main_struct.flag_run = 1;

    pthread_t control_tid;
    pthread_t menu_tid;

    pthread_create(&control_tid, NULL,(void *) controle_temp, (void *)&main_struct);
    // pthread_create(&menu_tid, NULL, (void *) write_menu, (void *)&main_struct);

    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    createImprimeDadosWindow();
    imprimeDados();

    pthread_join(control_tid, NULL);
    // pthread_join(menu_tid, NULL);

    return 0;
}

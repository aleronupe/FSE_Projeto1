#include <curses.h>
#include <locale.h>
#include <stdio.h>
#include "ncurses-menu.h"
#include "control.h"

/* Prints typical menus that you might see in games */
void write_menu(void *args){
    Arg_Struct *struct_received = (Arg_Struct *)args;
    int menu_ret = 1, menu_ret2 = 1, menu_temp = 1;
    char alts[][100] = {{"Start Game"},     /* Every menu needs an */
                         {"Load Game"},     /* array like these to */
                         {"Quit Game"},};   /* hold the entries.   */
    char alts2[][100] = {{"Slot 1"},
                         {"Slot 2"},
                         {"Slot 3"},
                         {"Return"},};
    char temps[3][100];

    setlocale (LC_CTYPE, "");

    initscr();                  /* Most of the below initialisers are */
    noecho();                   /* not necessary for this example.    */
    keypad (stdscr, TRUE);      /* It's just a template for a         */
    meta (stdscr, TRUE);        /* hypothetical program that might    */
    nodelay (stdscr, FALSE);    /* need them.                         */
    notimeout (stdscr, TRUE);
    raw();
    curs_set (0);

    do  /* This loop terminates when MAIN MENU returns 3, Quit Game.   */
    {   /* menu_ret is sent as the start value, to make the last entry */
        /* highlighted when you return to the main menu.               */

        snprintf(temps[0], 10, "%lf\n", struct_received->temp_ext);
        snprintf(temps[1], 10, "%f\n", struct_received->temp_int);
        if(struct_received->flag_insert_temp) 
            snprintf(temps[2], 10, "%f\n", struct_received->temp_ref_user); 
        else{
            snprintf(temps[2], 10, "%f\n", struct_received->temp_ref_pot); 
        }

        menu_ret = print_menu (2, 5, 3, 15,
                               "MAIN MENU", alts, menu_ret);

        menu_temp = print_menu (10, 5, 3, 15,
                               "Temperaturas", temps, menu_temp);

        if (menu_ret == 1)  /* This is just an example program. */
        {                   /* You can't start an actual game.  */
            mvprintw (5, 23, "Lol jk! This is a demo program.");
            mvprintw (6, 26, "Press any key to return.");
            getch();
        }

        else if (menu_ret == 2) /* If you select load game, a new    */
        {                       /* menu will show up without erasing */
            do                  /* the main menu. */
            {
                menu_ret2 = print_menu (6, 22, 4, 15,
                                        "SELECT SLOT", alts2, 1);
            }
            while (menu_ret2 != 4);
        }

        erase();    /* When you return from the SELECT SLOT menu,      */
    }               /* everything will be erased and MAIN MENU will be */
    while (menu_ret != 3); /* reprinted.                               */

    endwin();
}

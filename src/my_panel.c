#include "my_panel.h"

void print_panel(Arg_Struct *main_struct)
{

    WINDOW *w;
    int ch, i = 0, width = 30;
    char item[50];
    float temp_ref;

    if (main_struct->flag_insert_temp)
    {
        temp_ref = main_struct->temp_ref_user;
    }
    else
    {
        temp_ref = main_struct->temp_ref_pot;
    }

    initscr();               // initialize Ncurses
    w = newwin(50, 3, 1, 1); // create a new window
    box(w, 0, 0);            // sets default borders for the window

    wattroff(w, A_STANDOUT);
    sprintf(item, "TI: %f", main_struct->temp_int);
    mvwprintw(w, 1, 2, "%s", item);

    wattroff(w, A_STANDOUT);
    sprintf(item, "TE: %lf", main_struct->temp_ext);
    mvwprintw(w, 2, 2, "%s", item);

    wattroff(w, A_STANDOUT);
    sprintf(item, "TR: %f", temp_ref);
    mvwprintw(w, 3, 2, "%s", item);

    wrefresh(w); // update the terminal screen

    i = 0;
    noecho();        // disable echoing of characters on the screen
    keypad(w, TRUE); // enable keyboard input for the window.
    curs_set(0);     // hide the default screen cursor.

    // get the input
    while (main_struct->flag_run)
    {
        if (main_struct->flag_insert_temp)
        {
            temp_ref = main_struct->temp_ref_user;
        }
        else
        {
            temp_ref = main_struct->temp_ref_pot;
        }
        wrefresh(w); // update the terminal screen
    }

    delwin(w);
    endwin();
}

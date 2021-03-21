#include <curses.h>
#include <locale.h>
#include "ncurses-menu.h"

/* Prints typical menus that you might see in games */
int
main (int argc, char *argv[])
{
    int menu_ret = 1, menu_ret2 = 1;
    char alts[][100] = {{"Start Game"},     /* Every menu needs an */
                         {"Load Game"},     /* array like these to */
                         {"Quit Game"},};   /* hold the entries.   */
    char alts2[][100] = {{"Slot 1"},
                         {"Slot 2"},
                         {"Slot 3"},
                         {"Return"},};

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
        menu_ret = print_menu (2, 5, 3, 15,
                               "MAIN MENU", alts, menu_ret);

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
    return 0;
}
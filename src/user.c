#include "user.h"

void opcoes_usuario()
{
    int menu_ret = 1;
    char alts[][100] = {
        {"Utilizar TR do Potênciometro"}, /* Every menu needs an */
        {"Inserir TR"},  /* array like these to */
        {"Sair"},
    }; /* hold the entries.   */

    setlocale(LC_CTYPE, "");

    initscr();              /* Most of the below initialisers are */
    noecho();               /* not necessary for this example.    */
    keypad(stdscr, TRUE);   /* It's just a template for a         */
    meta(stdscr, TRUE);     /* hypothetical program that might    */
    nodelay(stdscr, FALSE); /* need them.                         */
    notimeout(stdscr, TRUE);
    raw();
    curs_set(0);

    do /* This loop terminates when MAIN MENU returns 3, Quit Game.   */
    {  /* menu_ret is sent as the start value, to make the last entry */
        /* highlighted when you return to the main menu.               */
        menu_ret = print_menu(7, 1, 3, 15,
                              "Temperatura de Referência", alts, menu_ret);

        if (menu_ret == 1) /* This is just an example program. */
        {                  /* You can't start an actual game.  */
        }

        else if (menu_ret == 2)
        {
        }


    }                     
    while (menu_ret != 3); 

    endwin();
}
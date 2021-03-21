#include <panel.h>
#include <ncurses.h>
#include <curses.h>
#include "control.h"

#define NLINES 10
#define NCOLS 40

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

void print_panel(Arg_Struct *main_struct)
{
    WINDOW *my_wins[3];
    PANEL *my_panels[3];
    PANEL *top;
    int ch;
    float temp_ref;

    /* Initialize curses */
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    if (main_struct->flag_insert_temp)
        temp_ref = main_struct->temp_ref_user;
    else
        temp_ref = main_struct->temp_ref_pot;

    /* Initialize all the colors */
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);

    init_wins(my_wins, main_struct->temp_ext, main_struct->temp_int, &temp_ref);

    /* Attach a panel to each window */   /* Order is bottom up */
    my_panels[0] = new_panel(my_wins[0]); /* Push 0, order: stdscr-0 */
    my_panels[1] = new_panel(my_wins[1]); /* Push 1, order: stdscr-0-1 */
    my_panels[2] = new_panel(my_wins[2]); /* Push 2, order: stdscr-0-1-2 */

    /* Set up the user pointers to the next panel */
    set_panel_userptr(my_panels[0], my_panels[1]);
    set_panel_userptr(my_panels[1], my_panels[2]);
    set_panel_userptr(my_panels[2], my_panels[0]);

    /* Update the stacking order. 2nd panel will be on top */
    update_panels();

    /* Show it on the screen */
    attron(COLOR_PAIR(4));
    mvprintw(LINES - 2, 0, "Use tab to browse through the windows (F1 to Exit)");
    attroff(COLOR_PAIR(4));
    doupdate();

    top = my_panels[2];
    while (main_struct->flag_run)
    {
        if (main_struct->flag_insert_temp)
            temp_ref = main_struct->temp_ref_user;
        else
            temp_ref = main_struct->temp_ref_pot;
        update_panels();
        doupdate();
    }
    endwin();
}

/* Put all the windows */
void init_wins(WINDOW **wins, double TE, float TI, float *TR)
{
    int x, y, i;
    char label[80];

    y = 2;
    x = 10;

    wins[0] = newwin(NLINES, NCOLS, y, x);
    sprintf(label, "Temperatura Externa: %lf", TE);
    win_show(wins[0], label, 1);
    y += 3;
    x += 7;

    wins[1] = newwin(NLINES, NCOLS, y, x);
    sprintf(label, "Temperatura Interna: %f", TI);
    win_show(wins[1], label, 2);
    y += 3;
    x += 7;

    wins[3] = newwin(NLINES, NCOLS, y, x);
    sprintf(label, "Temperatura de Referência: %f", *TR);
    win_show(wins[2], label, 3);
}

/* Show the window with a border and a label */
void win_show(WINDOW *win, char *label, int label_color)
{
    int startx, starty, height, width;

    getbegyx(win, starty, startx);
    getmaxyx(win, height, width);

    box(win, 0, 0);
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, ACS_HLINE, width - 2);
    mvwaddch(win, 2, width - 1, ACS_RTEE);

    print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label_color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{
    int length, x, y;
    float temp;

    if (win == NULL)
        win = stdscr;
    getyx(win, y, x);
    if (startx != 0)
        x = startx;
    if (starty != 0)
        y = starty;
    if (width == 0)
        width = 80;

    length = strlen(string);
    temp = (width - length) / 2;
    x = startx + (int)temp;
    wattron(win, color);
    mvwprintw(win, y, x, "%s", string);
    wattroff(win, color);
    refresh();
}
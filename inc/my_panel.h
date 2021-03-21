
#ifndef PANEL_H_
#define PANEL_H_

#include <panel.h>
#include <curses.h>
#include <locale.h>
#include <ncurses.h>
#include "control.h"

void print_panel(Arg_Struct *main_struct);
void init_wins(WINDOW **wins, double TE, float TI, float *TR);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#endif /* PANEL_H_ */

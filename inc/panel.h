
#ifndef PANLEL_H_
#define PANEL_H_

#include <panel.h>
#include <curses.h>
#include <locale.h>

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

#endif /* PANEL_H_ */


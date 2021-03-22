#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "control.h"

void inicia_menu(void *args);
void atualiza_temperaturas(Arg_Struct *struct_received);
void clear_menu(WINDOW *window);

#endif /* MENU_H_ */

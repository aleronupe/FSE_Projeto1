#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include "control.h"

void iniciaTelas();
void atualiza_temperaturas(void *args);
void opcoes_usuario();

#endif /* MENU_H_ */

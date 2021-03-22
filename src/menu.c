#include "menu.h"

WINDOW *windowImprimeDados;

Arg_Struct main_struct;

void createImprimeDadosWindow()
{
	int yMax, xMax;
	int tamXwindow = 50, posX = 2;
	int tamanhoYwindow = 5;

	windowImprimeDados = newwin(tamanhoYwindow, tamXwindow, 1, posX);

	box(windowImprimeDados, 0, 0);
	wrefresh(windowImprimeDados);
}

void imprimeDados(Arg_Struct *main_struct)
{

	while (main_struct->flag_run)
	{

		float temp_ref;

		if (main_struct->flag_insert_temp)
		{
			temp_ref = main_struct->temp_ref_user;
		}
		else
		{
			temp_ref = main_struct->temp_ref_pot;
		}

		wclear(windowImprimeDados);

		mvwprintw(windowImprimeDados, 1, 1, "Temperatura Interna = %f", main_struct->temp_int);
		mvwprintw(windowImprimeDados, 2, 1, "Temperatura Externa = %lf", main_struct->temp_ext);
		mvwprintw(windowImprimeDados, 3, 2, "Temperatura de Referência = %f", temp_ref);

		wrefresh(windowImprimeDados);
		sleep(1);
	}
	keypad(0);
	echo();
	nocbreak();
	endwin();
}

void atualiza_temperaturas(void *args)
{
	Arg_Struct *struct_received = (Arg_Struct *)args;
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);
	createImprimeDadosWindow();
	imprimeDados(struct_received);
}

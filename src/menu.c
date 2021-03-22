#include "menu.h"

WINDOW *windowImprimeDados;

Arg_Struct main_struct;

void createImprimeDadosWindow()
{

	windowImprimeDados = newwin(5, 20, 1, 1);
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

		mvwprintw(windowImprimeDados, 1, 1, "TI: %3.2f", main_struct->temp_int);

		mvwprintw(windowImprimeDados, 2, 1, "TE: %3.2lf", main_struct->temp_ext);

		mvwprintw(windowImprimeDados, 3, 1, "TR: %3.2f", temp_ref);

		wrefresh(windowImprimeDados);
		sleep(1);
	}
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

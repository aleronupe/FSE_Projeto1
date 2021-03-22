#include "menu.h"

WINDOW *windowImprimeDados;

Arg_Struct main_struct;

void createImprimeDadosWindow()
{

	windowImprimeDados = newwin(5, 20, 1, 1);
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
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

		attron(COLOR_PAIR(1));
		mvwprintw(windowImprimeDados, 1, 1, "TI: %3.2f", main_struct->temp_int);
		attroff(COLOR_PAIR(1));

		attron(COLOR_PAIR(2));
		mvwprintw(windowImprimeDados, 2, 1, "TE: %3.2lf", main_struct->temp_ext);
		attroff(COLOR_PAIR(2));

		attron(COLOR_PAIR(3));
		mvwprintw(windowImprimeDados, 3, 1, "TR: %3.2f", temp_ref);
		attroff(COLOR_PAIR(3));

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

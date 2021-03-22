#include "menu.h"

WINDOW *windowImprimeDados, *windowEntradaUsuario;

Arg_Struct main_struct;

void iniciaTelas()
{
	initscr();
	cbreak();
	keypad(stdscr, TRUE);
}

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
	createImprimeDadosWindow();
	imprimeDados(struct_received);
}

void printa_opcoes()
{
	mvwprintw(windowEntradaUsuario, 1, 1, "Escolha uma opção:");
	mvwprintw(windowEntradaUsuario, 2, 1, "1 - Inserir TR:");
	mvwprintw(windowEntradaUsuario, 3, 1, "2 - Utilizar TR do Potenciômetro:");
	mvwprintw(windowEntradaUsuario, 4, 1, "3 - Sair");
	wrefresh(windowEntradaUsuario);
}

void opcoes_usuario()
{
	int option;
	windowEntradaUsuario = newwin(5, 50, 6, 1);
	printa_opcoes();
	mvwscanw(windowEntradaUsuario, 5, 1, "%d", &option);

	if (option < 1 || option > 3)
	{
		wclear(windowEntradaUsuario);
		mvwprintw(windowEntradaUsuario, 1, 1, "Valor inválido, insira novamente.");
		wrefresh(windowEntradaUsuario);
		sleep(1);
	}
	wclear(windowEntradaUsuario);
	printa_opcoes();
	wrefresh(windowEntradaUsuario);
	mvwscanw(windowEntradaUsuario, 5, 1, "%d", &option);
}

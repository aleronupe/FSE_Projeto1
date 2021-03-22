#include "menu.h"

WINDOW *windowImprimeDados, *windowEntradaUsuario;

Arg_Struct main_struct;

void iniciaTelas()
{
	initscr();
	start_color();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
}

void createImprimeDadosWindow()
{
	windowImprimeDados = newwin(6, 40, 1, 1);
	box(windowImprimeDados, 0, 0);
	wrefresh(windowImprimeDados);
}

void imprimirRotulo(WINDOW *tmpJanela, int y, int x, char *sRotulo)
{

	wattron(tmpJanela, COLOR_PAIR(1));
	mvwprintw(tmpJanela, y, x, sRotulo);
	wattroff(tmpJanela, COLOR_PAIR(1));
}

void imprimeDados(Arg_Struct *main_struct)
{

	while (main_struct->flag_run)
	{

		float temp_ref;
		double intensity = main_struct->intensity;

		if (main_struct->flag_insert_temp)
		{
			temp_ref = main_struct->temp_ref_user;
		}
		else
		{
			temp_ref = main_struct->temp_ref_pot;
		}

		wclear(windowImprimeDados);

		wattron(windowImprimeDados, COLOR_PAIR(2));
		mvwprintw(windowImprimeDados, 1, 1, "TI: %3.2f", main_struct->temp_int);
		wattroff(windowImprimeDados, COLOR_PAIR(2));

		wattron(windowImprimeDados, COLOR_PAIR(3));
		mvwprintw(windowImprimeDados, 2, 1, "TE: %3.2lf", main_struct->temp_ext);
		wattroff(windowImprimeDados, COLOR_PAIR(3));

		wattron(windowImprimeDados, COLOR_PAIR(4));
		mvwprintw(windowImprimeDados, 3, 1, "TR: %3.2f", temp_ref);
		wattroff(windowImprimeDados, COLOR_PAIR(4));

		if (intensity > 0.0)
		{
			wattron(windowImprimeDados, COLOR_PAIR(5));
			mvwprintw(windowImprimeDados, 4, 1, "Resistor(ON): %3.2lf", intensity);
			wattroff(windowImprimeDados, COLOR_PAIR(5));

			wattron(windowImprimeDados, COLOR_PAIR(1));
			mvwprintw(windowImprimeDados, 5, 1, "Ventoinha (OFF)");
			wattroff(windowImprimeDados, COLOR_PAIR(1));
		}
		else if (main_struct->intensity < -40.0)
		{
			wattron(windowImprimeDados, COLOR_PAIR(1));
			mvwprintw(windowImprimeDados, 4, 1, "Resistor (OFF)");
			wattroff(windowImprimeDados, COLOR_PAIR(1));

			wattron(windowImprimeDados, COLOR_PAIR(5));
			mvwprintw(windowImprimeDados, 5, 1, "Ventoinha (ON): %3.2lf", (-1)*intensity);
			wattroff(windowImprimeDados, COLOR_PAIR(5));
		}
		else
		{
			wattron(windowImprimeDados, COLOR_PAIR(1));
			mvwprintw(windowImprimeDados, 4, 1, "Resistor (OFF)");
			wattroff(windowImprimeDados, COLOR_PAIR(1));

			wattron(windowImprimeDados, COLOR_PAIR(1));
			mvwprintw(windowImprimeDados, 5, 1, "Ventoinha (ON): %3.2lf", (-1)*intensity);
			wattroff(windowImprimeDados, COLOR_PAIR(1));
		}

		wrefresh(windowImprimeDados);
		usleep(600000);
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
	imprimirRotulo(windowEntradaUsuario, 1, 1, "");
	imprimirRotulo(windowEntradaUsuario, 2, 1, "Escolha uma opção:");
	imprimirRotulo(windowEntradaUsuario, 3, 1, "1 - Inserir TR:");
	imprimirRotulo(windowEntradaUsuario, 4, 1, "2 - Utilizar TR do Potenciômetro:");
	imprimirRotulo(windowEntradaUsuario, 5, 1, "3 - Sair");
	wrefresh(windowEntradaUsuario);
}

void opcoes_usuario(Arg_Struct *main_struct)
{
	int option;
	float temp_ref;
	windowEntradaUsuario = newwin(10, 50, 8, 1);

	while (main_struct->flag_run)
	{
		int invalid_temp = 1;
		printa_opcoes();
		echo();
		wmove(windowEntradaUsuario, 6, 1);
		wscanw(windowEntradaUsuario, "%d", &option);
		// mvwscanw(windowEntradaUsuario, 5, 1, "%d", &option);
		// mvwprintw(windowEntradaUsuario, 6, 1, "%d", option);

		switch (option)
		{
		case 1:
			while (invalid_temp)
			{
				wclear(windowEntradaUsuario);
				imprimirRotulo(windowEntradaUsuario, 2, 1, "Digite a Temperatura de Referência:");
				wmove(windowEntradaUsuario, 4, 1);
				wscanw(windowEntradaUsuario, "%f", &temp_ref);
				if (temp_ref > 100 || temp_ref < main_struct->temp_ext)
				{
					wclear(windowEntradaUsuario);
					imprimirRotulo(windowEntradaUsuario, 3, 1, "Temperatura inválida");
					wrefresh(windowEntradaUsuario);
					sleep(1);
				}
				else
				{
					main_struct->temp_ref_user = temp_ref;
					main_struct->flag_insert_temp = 1;
					invalid_temp = 0;
					sleep(1);
					wclear(windowEntradaUsuario);
				}
			}
			break;
		case 2:
			wclear(windowEntradaUsuario);
			main_struct->flag_insert_temp = 0;
			mvwprintw(windowEntradaUsuario, 3, 1, "Valor Atualizado");
			wrefresh(windowEntradaUsuario);
			sleep(1);
			wclear(windowEntradaUsuario);
			break;
		case 3:
			main_struct->flag_run = 0;
			wclear(windowEntradaUsuario);
			echo();
			nocbreak();
			endwin();
			break;
		default:
			wclear(windowEntradaUsuario);
			mvwprintw(windowEntradaUsuario, 3, 1, "Valor inválido, insira novamente.");
			wrefresh(windowEntradaUsuario);
			sleep(1);
			wclear(windowEntradaUsuario);
		}
	}
}

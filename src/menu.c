#include "menu.h"

#define WIDTH 70
#define HEIGHT 13

WINDOW *window;
int is_reference_screen;

char *options[] = {
	"Inserir Temperatura de Referência",
	"Utilizar Temperatura de Referência do Potenciômetro",
	"Sair",
};

int startx = 0;
int starty = 0;
int n_options = sizeof(options) / sizeof(char *);

void clear_reference(Arg_Struct *struct_received)
{
	struct_received->flag_insert_temp = 0;
	refresh();
}

void print_input(WINDOW *window, int y, int x, char *input)
{
	wattron(window, COLOR_PAIR(1));
	mvwprintw(window, y, x, input);
	wattroff(window, COLOR_PAIR(1));
}

void set_reference(Arg_Struct *struct_received)
{
	WINDOW *reference_window;

	float reference;
	int max_height, max_width;
	int _startx, _starty;

	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	getmaxyx(stdscr, max_height, max_width);

	_starty = (LINES - max_height) / 2;
	_startx = (COLS - max_width) / 2;
	refresh();

	reference_window = newwin(HEIGHT, WIDTH, _starty, _startx);

	wborder(reference_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);

	print_input(reference_window, _starty + 1, _startx + 1, "Insira Temperatura de Referência: ");

	echo();
	wmove(reference_window, _starty + 1, _startx + 13);
	wscanw(reference_window, "%f", &reference);

	struct_received->temp_ref_user = reference;
	struct_received->flag_insert_temp = 1;
	is_reference_screen = 0;

	clear_menu(reference_window);
}

void write_menu(WINDOW *window, int highlight)
{
	int x = 1, y = 1, i;

	box(window, 0, 0);
	for (i = 0; i < n_options; i++)
	{
		if (highlight == i + 1)
		{
			wattron(window, A_REVERSE);
			mvwprintw(window, y, x, "\t%s", options[i]);
			wattroff(window, A_REVERSE);
		}
		else
		{
			mvwprintw(window, y, x, "\t%s", options[i]);
		}
		++y;
	}
	wrefresh(window);
}

void clear_menu(WINDOW *window_param)
{
	wclear(window_param);
	wborder(window_param, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(window_param);
	delwin(window_param);
}

void atualiza_temperaturas(Arg_Struct *struct_received)
{
	if (is_reference_screen == 1)
	{
		return;
	}

	char *on = "ON ";
	char *off = "OFF";

	float temp_ref;
	if (struct_received->flag_insert_temp)
	{
		temp_ref = struct_received->temp_ref_user;
	}
	else
	{
		temp_ref = struct_received->temp_ref_pot;
	}

	mvwprintw(window, 6, 1, "\tTI: %4.2f", struct_received->temp_int);
	mvwprintw(window, 7, 1, "\tTR: %4.2f", data.reference, data.control);
	mvwprintw(window, 8, 1, "\tTE: %4.2lf", struct_received->temp_ext);
	// mvwprintw(window, 9, 1, "\tRESISTOR: %s", data.control > 0 ? on : off);
	// mvwprintw(window, 10, 1, "\tCOOLER: %s", data.control < -40 ? on : off);
	wrefresh(window);
}

void inicia_menu(void *args)
{
	int highlight = 1;
	int option = 0;
	int c;

	Arg_Struct *struct_received = (Arg_Struct *)args;

	initscr();
	start_color();
	clear();
	noecho();
	cbreak();
	curs_set(0);

	window = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(window, TRUE);
	refresh();

	do
	{
		write_menu(window, highlight);
		c = wgetch(window);

		switch (c)
		{
		case KEY_UP:
			if (highlight == 1)
				highlight = n_options;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_options)
				highlight = 1;
			else
				++highlight;
			break;
		case 10:
			option = highlight;
			if (option == 1)
			{
				clear_menu(window);
				is_reference_screen = 1;
				set_reference(struct_received);
				window = newwin(HEIGHT, WIDTH, starty, startx);
				keypad(window, TRUE);
				refresh();
			}

			if (option == 2)
			{
				clear_reference(struct_received);
			}

			if (option == 3)
			{
				// quit_handler();
				struct_received->flag_run = 0;
				refresh();
				endwin();
				all_off();
				exit(0);
			}

			break;
		default:
			refresh();
			break;
		}
	} while (1);
}
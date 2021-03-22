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
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	init_pair(2,COLOR_YELLOW,COLOR_BLACK);
	init_pair(3,COLOR_BLUE,COLOR_BLACK);
	init_pair(4,COLOR_MAGENTA,COLOR_BLACK);

}

void createImprimeDadosWindow()
{
	windowImprimeDados = newwin(5, 20, 1, 1);
	box(windowImprimeDados, 0, 0);
	wrefresh(windowImprimeDados);
}

void imprimirRotulo(WINDOW *tmpJanela,int y, int x, char *sRotulo){

    wattron(tmpJanela,COLOR_PAIR(1));
    mvwprintw(tmpJanela,y,x,sRotulo);
    wattroff(tmpJanela,COLOR_PAIR(1));

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

    	wattron(windowImprimeDados,COLOR_PAIR(2));
		mvwprintw(windowImprimeDados, 1, 1, "TI: %3.2f", main_struct->temp_int);
    	wattroff(windowImprimeDados,COLOR_PAIR(2));

    	wattron(windowImprimeDados,COLOR_PAIR(3));
		mvwprintw(windowImprimeDados, 2, 1, "TE: %3.2lf", main_struct->temp_ext);
    	wattroff(windowImprimeDados,COLOR_PAIR(3));

    	wattron(windowImprimeDados,COLOR_PAIR(4));
		mvwprintw(windowImprimeDados, 3, 1, "TR: %3.2f", temp_ref);
    	wattroff(windowImprimeDados,COLOR_PAIR(4));


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
	imprimirRotulo(windowEntradaUsuario, 1, 1, "Escolha uma opção:");
	imprimirRotulo(windowEntradaUsuario, 2, 1, "1 - Inserir TR:");
	imprimirRotulo(windowEntradaUsuario, 3, 1, "2 - Utilizar TR do Potenciômetro:");
	imprimirRotulo(windowEntradaUsuario, 4, 1, "3 - Sair");
	wrefresh(windowEntradaUsuario);
}

void opcoes_usuario(Arg_Struct *main_struct)
{
	int option;
	float temp_ref;
	windowEntradaUsuario = newwin(5, 50, 6, 1);

	while(1){
		printa_opcoes();
		echo();
		wmove(windowEntradaUsuario, 7, 1);
		wscanw(windowEntradaUsuario,"%d", &option); 
		// mvwscanw(windowEntradaUsuario, 5, 1, "%d", &option);
		// mvwprintw(windowEntradaUsuario, 6, 1, "%d", option);

		switch(option){
			case 1:
				wclear(windowEntradaUsuario);
				imprimirRotulo(windowEntradaUsuario, 1, 1, "Digite a Temperatura de Referência:");
				wmove(windowEntradaUsuario, 2, 1);
				wscanw(windowEntradaUsuario,"%f", &temp_ref);
				main_struct->flag_insert_temp = 1;
				main_struct->temp_ref_user = temp_ref;
				sleep(1);
				wclear(windowEntradaUsuario);
				break;
			case 2:
				wclear(windowEntradaUsuario);
				main_struct->flag_insert_temp = 1;
				mvwprintw(windowEntradaUsuario, 1, 1, "Valor Atualizado");
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
				mvwprintw(windowEntradaUsuario, 1, 1, "Valor inválido, insira novamente.");
				wrefresh(windowEntradaUsuario);
				sleep(1);
				wclear(windowEntradaUsuario);
		}
	}
}

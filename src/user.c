#include "user.h"

WINDOW *windowEntradaUsuario;

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
    windowEntradaUsuario = newwin(5, 20, 6, 1);
    printa_opcoes();
    mvwscanw(windowEntradaUsuario, 5, 1, "%d", &option);

    if (option < 1 || option > 3)
    {
        wclear(windowImprimeDados);
        mvwprintw(windowEntradaUsuario, 1, 1, "Valor inválido, insira novamente.");
        wrefresh(windowEntradaUsuario);
        sleep(1)
    }
    wclear(windowImprimeDados);
    printa_opcoes();
    wrefresh(windowEntradaUsuario);
    mvwscanw(windowEntradaUsuario, 5, 1, "%d", &option);
    endwin();
}
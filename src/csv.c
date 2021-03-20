#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void abre_ou_cria_csv()
{
    FILE *fpt;

    fpt = fopen("dados.csv", "w");
    fprintf(fpt, "Data/Hora, TI, TE, TR, Intensidade\n");
    fclose(fpt);
}

void escreve_csv(float TI, double TE, float TR, double inten)
{
    FILE *fpt;
    fpt = fopen("dados.csv", "a");

    time_t rawtime;
    struct tm *info;
    char dataHora[80];

    time(&rawtime);

    info = localtime(&rawtime);

    strftime(dataHora, 80, "%d/%m/%Y %H:%M:%S", info);

    fprintf(fpt, "%s, %f, %lf, %f, %lf\n", dataHora, TI, TE, TR, inten);
    fclose(fpt);
}
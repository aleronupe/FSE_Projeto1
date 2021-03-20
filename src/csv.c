#include <stdio.h>
#include <stdlib.h>

void abre_ou_cria_csv()
{
    FILE *fpt;

    fpt = fopen("dados.csv", "w");
    fprintf(fpt,"Data/Hora, TI, TE, TR, Intensidade\n");
    fclose(fpt);
}

void escreve_csv(unsigned int dataHora, float TI, double TE, float TR, double inten)
{
    FILE *fpt;
    
    fpt = fopen("dados.csv", "a");
    fprintf(fpt,"%d, %f, %lf, %f, %lf\n", dataHora, TI, TE, TR, inten);
    fclose(fpt);
}
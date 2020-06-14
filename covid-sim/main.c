#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "local.h"
#include "pessoa.h"

#define MAX 80

void simularDia(ppessoa p, int dia, int *n, plocal e) {
    
    

    printf("\n*********************************************** DIA %d DA SIMULACAO! ***********************************************\n", dia);
    modeloPropagacao(p,dia);
    taxaDisseminacao(p,e,*(n));
    printf("\n********************************************* FIM DO DIA %d DA SIMULACAO! ******************************************\n", dia);

}

int main(int argc, char** argv) {

    initRandom();

    //A simulação começa no dia 0 (zero)
    int nDia = 0;
    local *espaco = NULL;

    char bin[] = ".bin";
    char txt[] = ".txt";
    char binFile[MAX], pplFile[MAX];

    printf("Ficheiro Binario >>> ");
    scanf(" %s", &binFile);
    strcat(binFile, bin);

    printf("\n\nFicheiro Pessoas >>> ");
    scanf(" %s", &pplFile);
    strcat(pplFile, txt);

    system("CLS");

    //NUMERO DE ESPAÇOS(LOCAIS) NO FICHEIRO BINARIO
    int total = nEspacos(binFile);
    //LER O FICHEIRO BINARIO E GUARDAR DADOS NO VETOR DINAMICO 'espaco'
    espaco = readBinData(binFile, &total);

    /*espaco[0].capacidade = 1;
    espaco[1].capacidade = 2;
    espaco[2].capacidade = 2;
    espaco[3].capacidade = 1;*/



    ppessoa lista = carregarPessoas(pplFile, espaco, total);
    
    //validaPessoas(lista);

    //mostraEspacos(espaco, total);

    int cmd;

    do {
        printf("\t\t************** MENU **************\n"
                "\t\t*                                *\n"
                "\t\t*      [1] Mostrar Pacientes     *\n"
                "\t\t*      [2] Mostrar Locais        *\n"
                "\t\t*      [3] Simulacao             *\n"
                "\t\t*      [4] lalalala              *\n"
                "\t\t*      [5] Sair                  *\n"
                "\t\t*                                *\n"
                "\t\t**********************************\n\n");
        printf(">>>");
        scanf("%d", &cmd);

        switch (cmd) {
            case 1:
                system("CLS");
                mostraPessoas(lista);
                printf("\n\n\n");
                break;
            case 2:
                system("CLS");
                mostraEspacos(espaco, total);
                printf("\n\n\n");
                break;
            case 3:
                //system("CLS");
                simularDia(lista, nDia,&total,espaco);
                nDia++;
                printf("\n\n\n");
                break;
        }

    } while (cmd != 5);

    libertaListaPessoas(lista);
    free(espaco);
    return 0;
}
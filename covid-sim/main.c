#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "local.h"
#include "pessoa.h"



int main(int argc, char** argv) {

    initRandom();

    ppessoa lista = lerTXTpessoas();


    local *espaco = NULL;
    char binFile[25];
    printf("Ficheiro Binario:");
    scanf(" %s", &binFile); //TODO->fazer concatenação com a extensão .bin

    system("CLS");

    //NUMERO DE ESPAÇOS(LOCAIS) NO FICHEIRO BINARIO
    int total = nEspacos(binFile);
    //LER O FICHEIRO BINARIO E GUARDAR DADOS NO VETOR DINAMICO 'espaco'
    espaco = readBinData(binFile, &total);


    /*
    espaco[0].capacidade = 1;
    espaco[1].capacidade = 2;
    espaco[2].capacidade = 2;
    espaco[3].capacidade = 1;
     */

    mostraEspacos(espaco, total);



    /*printf("TOTAL ESPACOS->%d\n\n", total);
    int check = verificaID(espaco, total);
    if (check)
        printf("SEM ERROS! %d\n\n", check);
    else
        printf("COM ERROS! %d\n\n", check);*/



    alocaPessoas(lista, espaco, total);
    //mostraPessoas(lista);

    printf("\nxau\n\n");
    int cmd;

    do {
        printf("\t\t***************MENU***************\n"
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
        }

    } while (cmd != 5);
    
    free(lista);
    free(espaco);

    
    return 0;

}
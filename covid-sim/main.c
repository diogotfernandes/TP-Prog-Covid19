#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "local.h"
#include "pessoa.h"
#include "simulacao.h"

#define MAX 80

int main(int argc, char** argv) {

    initRandom();
    //A simulação começa no dia 1 (um)
    int nDia = 1;
    local *espaco = NULL;

    char bin[] = ".bin";
    char txt[] = ".txt";
    char binFile[MAX], pplFile[MAX];

    printf("Ficheiro Binario >>> ");
    scanf(" %s", &binFile);
    strcat(binFile, bin);       //JUNTAR .bin AO FICHEIRO

    printf("\n\nFicheiro Pessoas >>> ");
    scanf(" %s", &pplFile);
    strcat(pplFile, txt);       //JUNTAR .txt AO FICHEIRO


    //NUMERO DE ESPAÇOS(LOCAIS) NO FICHEIRO BINARIO
    int total = nEspacos(binFile);
    //LER O FICHEIRO BINARIO E GUARDAR DADOS NO VETOR DINAMICO 'espaco'
    espaco = readBinData(binFile, &total);

    /*espaco[0].capacidade = 23;
    espaco[1].capacidade = 23;
    espaco[2].capacidade = 30;
    espaco[3].capacidade = 25;*/

    if (!verificaID(espaco, total)) {
        printf("\n\aFALHOU A VERIFICACAO DOS ESPACOS!\n");
        exit(EXIT_FAILURE);
        getchar();
    }

    ppessoa lista = carregarPessoas(pplFile, espaco, total);

    system("cls");

    int cmd;

    do {
        printf("    \t\t\t\t\t.bin: %s\n"
                "   \t\t\t\t\t.txt: %s\n"
                "\n\t\t\t\t************** MENU **************\n"
                "\t\t\t\t*      [1] Mostrar Pessoas       *\n"
                "\t\t\t\t*      [2] Mostrar Locais        *\n"
                "\t\t\t\t*      -------SIMULACAO--------  *\n"
                "\t\t\t\t*      [4] Avancar 1 Dia         *\n"
                "\t\t\t\t*      [5] Estatisticas          *\n"
                "\t\t\t\t*      [6] Adicionar Doente      *\n"
                "\t\t\t\t*      [7] Transferir Doente     *\n"
                "\t\t\t\t*      [8] Terminar Simulacao    *\n"
                "\t\t\t\t*                                *\n"
                "\t\t\t\t*    >>>  ", binFile, pplFile);
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
            case 4:
                system("CLS");
                taxaDisseminacao(lista, espaco, total);
                simularDia(lista, nDia, &total, espaco);

                nDia++;
                printf("\n\n\n");
                break;
            case 5:
                system("CLS");
                printf("ESTATISTICAS\n");
                pessoasLocal(lista, espaco, total);
                printf("\n");
                break;
            case 6:
                system("CLS");
                printf("ADD\n");
                lista = novoDoente(lista, espaco, total);
                break;
            case 7:
                system("CLS");
                printf("TRANSFERIR\n\n\n");
                break;
        }

    } while (cmd != 8);

    char popTXT[MAX];
    printf("\nNome Ficheiro Populacao Final >>> ");
    scanf(" %s", &popTXT);
    escreveFicheiroPop(lista, popTXT);
    escreveReport(lista, espaco, total,nDia);

    libertaListaPessoas(lista);
    free(espaco);
    return 0;
}
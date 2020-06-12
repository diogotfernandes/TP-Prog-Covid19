#include <stdio.h>
#include <stdlib.h>
#include "local.h"

/*******************************ESPAÇOS*******************************/

//CONTA TOTAL DE ESPAÇOS

int nEspacos(char * nome) {
    FILE *f;
    local e;
    int num = 0;

    f = fopen(nome, "rb");

    if (f == NULL) {
        printf("Erro no acesso ao ficheiro de leitura!\n");
        return 0;
    }
    fseek(f, 0, SEEK_END);

    num = ftell(f) / sizeof (local);

    //printf("Numero de Espaços: %d", num);

    fclose(f);
    return num;
}

//LER VETOR DINÂMICO DOS ESPAÇOS

void mostraEspacos(local e[], int n) {
    for (int i = 0; i < n; i++) {
        printf("ID   -> \t[%d]\n", e[i].id);
        printf("Cap. -> \t[%d]\n", e[i].capacidade);
        for (int j = 0; j < 3; j++) {
            if (e[i].liga[j] != -1)
                printf("Liga[%d] -> \t[%d]\n", j, e[i].liga[j]);
        }
        printf("\n");
    }
}

//LÊ FICHEIRO BINÁRIO

local* readBinData(char *nome, int *total) {
    FILE *f;
    local *e;

    f = fopen(nome, "rb");
    if (f == NULL) {
        printf("\a");
        printf("Erro no acceso ao Ficheiro [%s]\n", nome);
        *total = 0;
        exit(EXIT_FAILURE);
    }

    e = malloc(sizeof (local)* *total);
    if (e == NULL) {
        printf("Erro na alocação de memória dinâmica\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *total; i++) {
        fread(e, sizeof (local), *total, f);
    }
    fclose(f);
    return e;
}
//TODO: VERIFICAR LIGAÇÕES!!
//VERIFICA ERROS NOS ESPAÇOS (ID's NEGATIVOS E ID's REPETIDOS)
//Sem erros -> 1
//Com erros -> 0

int verificaID(local e[], int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        if (e[i].id < 0) //PROCURA ID'S NEGATIVOS
            return 0;
        for (j = i + 1; j < n; j++) //PROCURA ID'S REPETIDOS
        {
            if (e[i].id == e[j].id)
                return 0;
        }
    }
    return 1;
}
/*********************************************************************/


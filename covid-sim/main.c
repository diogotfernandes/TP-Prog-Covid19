#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 25

typedef struct sala local, *plocal;

struct sala {
    int id; // id numerico do local
    int capacidade; // capacidade maxima
    int liga[3]; // id das ligacoes (-1 nos casos nao usados)
};

typedef struct individuo pessoa, *ppessoa; //lalal

struct individuo {
    char id[MAX]; //id, alfanumerico, do individuo
    int idade;
    char estado[1];
    int diasDoente;
    struct individuo *next;
};

ppessoa lerPessoas();

int nEspacos(char * nome);

int procuraPorID(local e[], int id, int total);
int verificaLigacoes(local e[], int total);

int verificaID(local e[], int n);
local* readBinData(char *nome, int *total);

int main(int argc, char** argv) {
    ppessoa listaPessoas;
    
    


    lerPessoas();

    local *espaco = NULL;
    char binFile[25];
    printf("File Name:");
    scanf(" %s", &binFile); //TODO->fazer concatenação com a extensão .bin
    system("CLS");



    int total = nEspacos(binFile); //NUMERO DE ESPAÇOS(LOCAIS) NO FICHEIRO BINARIO
    espaco = readBinData(binFile, &total); //LER O FICHEIRO BINARIO E GUARDAR DADOS NUM VETOR DINAMICO

    //espaco[2].liga[0] = 9;
    //espaco[2].id = -1;
    int nLigacoes;
    nLigacoes = verificaLigacoes(espaco, total);
    printf("TOTAL ESPACOS->%d\n\n", total);
    printf("TOTAL LIGACOES->%d\n\n", nLigacoes);

    int check = verificaID(espaco, total);
    if (check)
        printf("SEM ERROS! %d\n\n", check);
    else
        printf("COM ERROS! %d\n\n", check);



    //printf("TOTAL =%d\n", total);
    for (int i = 0; i < total; i++) {
        printf("ID   -> \t[%d]\n", espaco[i].id);
        printf("Cap. -> \t[%d]\n", espaco[i].capacidade);
        for (int j = 0; j < 3; j++) {
            if (espaco[i].liga[j] != -1)
                printf("Liga[%d] -> \t[%d]\n", j, espaco[i].liga[j]);

        }
        printf("\n");
    }
    return 0;
}

ppessoa lerPessoas() {
    ppessoa novo, anterior = NULL, lista = NULL;
    pessoa aux;

    FILE *fr = fopen("pessoasA.txt", "r");
    if (!fr) {
        printf("Erro a ler o ficheiro.\n");
        return NULL;
    }

    aux.next = NULL;

    while (fscanf(fr, "%s %d %s", aux.id, &aux.idade, aux.estado) == 3) {
        printf("%s\n", aux.id);
        printf("%d\n", aux.idade);
        printf("%s\n", aux.estado);

        int n;

        if (*aux.estado == 'D') {
            fscanf(fr, "%d", &n);
            printf("%d\n\n", n);
        }

        //aux.diasDoente = &n;

        printf("%d\n", aux.diasDoente);

        novo = (ppessoa) malloc(sizeof (pessoa));
        if (!novo) {
            printf("Erro a reservar memoria.\n");
            break;
        }
        *novo = aux;
    }

}

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

int verificaLigacoes(local e[], int total) {
    int i, k, j, id = 0, index = 0, ligacoes = 0;

    for (i = 0; i < total; i++) {
        for (j = 0; j < 3; j++) {
            id = e[i].liga[j];
            if (id != -1) {
                index = procuraPorID(e, id, total);
                printf("ID A PROCURAR - [%d] \t ESTA NO INDICE - [%d]\n", id, index);
                for (k = 0; k < 3; k++) {
                    if (e[index].liga[k] == e[i].id) {
                        ligacoes++;
                        printf("[%d]==[%d]\n", e[index].liga[k], e[i].id);
                        printf("EXISTE LIGACAO\n");
                    } else
                        printf("NA0 EXISTE LIGACAO\n");
                }
                printf("--------------------------\n");
            }
        }
    }
    return ligacoes;
}

//DADO UM ID, VAI ENCONTRAR QUAL É O SEU INDEX NO VETOR 
//RECEBE O VETOR E O ID A PROCURAR, RETORNA O INDEX DAQUELE ID

int procuraPorID(local e[], int id, int total) {
    int i, j;

    for (i = 0; i < total; i++) {
        for (j = 0; j < 3; j++) {
            if (e[i].id == id) {
                return i;
            }
        }
    }
    return -1;
}

/*int checkConn(local e[], int index, int idAprocurar, int *total) 
{
    int j, i, idDestino;
    int check = 0;
    //printf("Sou o e[%d] e vou procurar o ID[%d]\n", index, idAprocurar);

    for (i = 0; i < *total; i++) {
        if (e[i].id == idAprocurar) {
            idDestino = e[i].id;
            for (j = 0; j < 3; j++) {
                if (e[i].liga[j] != -1) {
                    printf("[%d]\t", e[i].liga[j]);
                    if (e[i].liga[j] == e[index].id) {
                        printf("SUCCESS\n\n");
                        check = 1;
                        break;
                    } else
                        check = 0;
                }
            }
            printf("-----------------------------------\n");
        }
    }
    return check;
}*/

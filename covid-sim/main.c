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

ppessoa lerTXTpessoas();
void mostraPessoas(ppessoa lista);


int nEspacos(char * nome);
int verificaID(local e[], int n);
local* readBinData(char *nome, int *total);
void mostraEspacos(local e[], int total);

int main(int argc, char** argv) {

    ppessoa lista = lerTXTpessoas();
    mostraPessoas(lista);

    local *espaco = NULL;
    char binFile[25];
    printf("File Name:");
    scanf(" %s", &binFile); //TODO->fazer concatenação com a extensão .bin

    system("CLS");

    //NUMERO DE ESPAÇOS(LOCAIS) NO FICHEIRO BINARIO
    int total = nEspacos(binFile);
    //LER O FICHEIRO BINARIO E GUARDAR DADOS NO VETOR DINAMICO 'espaco'
    espaco = readBinData(binFile, &total);

    mostraEspacos(espaco,total); 

    printf("TOTAL ESPACOS->%d\n\n", total);
    int check = verificaID(espaco, total);
    if (check)
        printf("SEM ERROS! %d\n\n", check);
    else
        printf("COM ERROS! %d\n\n", check);

    return 0;

}
/*******************************PESSOAS*******************************/
//LER LISTA CRIADA A PARTIR DO FICHEIRO DE PESSOAS

void mostraPessoas(ppessoa lista) {
    ppessoa aux = lista;

    while (aux) {
        printf("ID:\t%s\n", aux->id);
        printf("IDADE:\t%d\n", aux->idade);
        printf("ESTADO:\t%s\n", aux->estado);
        if (strcmp(aux->estado, "D") == 0)
            printf("DIAS :\t%d\n", aux->diasDoente);
        printf("------------------------------------\n");

        aux = aux -> next;
    }
}

ppessoa lerTXTpessoas() {
    ppessoa novo, anterior = NULL, lista = NULL;
    pessoa aux;

    FILE *fr = fopen("pessoasA.txt", "r");
    if (!fr) {
        printf("\a");
        printf("Erro a ler o ficheiro.\n");
        return NULL;
    }

    aux.next = NULL;

    while (fscanf(fr, "%s %d %s", aux.id, &aux.idade, aux.estado) != EOF) { //while (!feof(in_file)) -> Devolve valor != 0 se o indicador de final de ficheiro foi atingido
        if (strcmp(aux.estado, "D") == 0) { // if Return value = 0 then it indicates str1 is equal to str2.
            fscanf(fr, "%d", &aux.diasDoente);
        }
        novo = (ppessoa) malloc(sizeof (pessoa));
        if (!novo) {
            printf("Erro a reservar memoria.\n");
            break;
        }
        *novo = aux;
        //1ª iteração, a lista vai estar a NULL
        if (lista == NULL)
            lista = novo;
        else
            anterior->next = novo;
        anterior = novo;
    }
    fclose(fr);
    return lista;
}
/*********************************************************************/


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
    for (i = 0; i < n; i++) 
    {
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
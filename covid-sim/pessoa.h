#ifndef PESSOAS_H
#define PESSOAS_H
#include "local.h"

typedef struct individuo pessoa, *ppessoa; //lalal
struct individuo {
    char id[25]; //id, alfanumerico, do individuo
    int idade;
    char estado[1];
    int diasDoente;
    ppessoa next;
    plocal local;
};

//Lê ficheiro de texto, distribui as pessoas pelos locais existentes (com espaço) e guarda numa lista ligada
ppessoa carregarPessoas(char *file,plocal e, int nEspacos);
//Mostra a lista de pessoas
void mostraPessoas(ppessoa lista);
//Distribui as pessoas pelos locais exisitentes
//void alocaPessoas(ppessoa lista, local e[], int n);
//Liberta memória ocupada pelos elementos da lista
void libertaListaPessoas(ppessoa p);



int validaPessoa(char *id,int idade, char *estado, int diasDoente, ppessoa p);



#endif /* PESSOAS_H */


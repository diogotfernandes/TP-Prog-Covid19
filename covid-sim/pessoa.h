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

//Lê ficheiro de texto e guarda numa lista ligada
ppessoa lerTXTpessoas();
//Mostra a lista de pessoas
void mostraPessoas(ppessoa lista);
//Distribui as pessoas pelos locais exisitentes
void alocaPessoas(ppessoa lista, local e[], int n);


#endif /* PESSOAS_H */


#ifndef PESSOAS_H
#define PESSOAS_H
#include "local.h"

typedef struct individuo pessoa, *ppessoa;
struct individuo {
    char id[25]; //id, alfanumerico, do individuo
    int idade;
    char estado[1];
    int diasDoente;
    ppessoa next;	//PONTEIRO PARA PRÓXIMO ELEMENTE - SE FOR IGUAL A NULL, A LISTA TERMINOU
    plocal local;	//PONTEIRO PARA O LOCAL ATRIBUIDO A CADA PESSOA
};

//Lê ficheiro de texto, distribui as pessoas pelos locais existentes (com espaço) e guarda numa lista ligada
ppessoa carregarPessoas(char *file,plocal e, int nEspacos);

//Mostra a lista de pessoas
void mostraPessoas(ppessoa lista);

//Liberta memória ocupada pelos elementos da lista
void libertaListaPessoas(ppessoa p);

//VALIDA CADA PESSOA, ANTES DE TRANSFERIR PARA LISTA LIGADA
int validaPessoa(char *id,int idade, char *estado, int diasDoente, ppessoa p);


#endif /* PESSOAS_H */


#ifndef LOCAL_H
#define LOCAL_H

typedef struct sala local, *plocal;
struct sala {
    int id; // id numerico do local
    int capacidade; // capacidade maxima
    int liga[3]; // id das ligacoes (-1 nos casos nao usados)
};

//Conta número de espaços no ficheiro binário
int nEspacos(char * nome);

//VERIFICA ID'S REPETIDO E/OU NEGATIVOS
int verificaID(local e[], int n);

//Lê ficheiro binário e guarda num vetor dinâmico
local* readBinData(char *nome, int *total);

//Mostra o vetor de locais
void mostraEspacos(local e[], int total);


#endif /* LOCAL_H */


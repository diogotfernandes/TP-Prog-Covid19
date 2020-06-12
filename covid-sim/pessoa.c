#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pessoa.h"
#include "utils.h"

/*******************************PESSOAS*******************************/
//LER LISTA CRIADA A PARTIR DO FICHEIRO DE PESSOAS
void mostraPessoas(ppessoa lista) {
    ppessoa aux = lista;

    while (aux) {
        printf("ID -> \t\t%s\n", aux->id);
        printf("Idade -> \t%d\n", aux->idade);
        printf("Estado -> \t%s\n", aux->estado);
        if (strcmp(aux->estado, "D") == 0)
            printf("Dias -> \t%d\n", aux->diasDoente);
        if (aux->local != NULL)
            printf("LocalID -> \t%d\n\n", aux->local->id);
        else
            printf("Sem Local\n\n");
        printf("------------------------------------\n\n");

        aux = aux -> next;
    }
}

//LER FICHEIRO DE TEXTO E CRIAR LISTA LIGADA
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
    aux.local = NULL;

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

//ATRIBUIR LOCAIS ÀS PESSOAS
//CASSO NÃO EXISTA LOCAL C/ ESPAÇO, APONTA PARA NULL (FICA SEM LOCAL PARA A SIMULAÇÃO)
void alocaPessoas(ppessoa lista, plocal e, int n) {
    int i, num, haEspaco = 0;
    //ppessoa aux = lista;

    while (lista) {

        do {
            num = intUniformRnd(0, n - 1);
            //printf("[%d]", num);

            if (e[num].capacidade > 0) {
                lista->local = &e[num];
                e[num].capacidade--;
                printf("%d\n", e[num].capacidade);
                break;
            }

            int contaVazios = 0;

            for (i = 0; i < n; i++) {
                if (e[i].capacidade > 0)
                    haEspaco = 1;
                else
                    contaVazios++;
            }

            if (contaVazios == n) {
                lista->local = NULL;
                break;
            }

        } while (haEspaco != 0);

        lista = lista->next;
    }
}

/*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "pessoa.h"
#include "utils.h"


/*ESTADO POSSÍVEIS
 * [S] -> Saudável
 * [D] -> Doente
 * [I] -> Infetado
 * 
 * [C] -> Curado (após n dias ou c/ prob recuperaçao)
 * [I] -> Imune (prob de ficar imune (20%; após ser curado))
 */

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
        printf("LocalID -> \t%d\n", aux->local->id);
        printf("------------------------------------\n\n");

        aux = aux -> next;
    }
}

//LER FICHEIRO DE TEXTO E CRIAR LISTA LIGADA

ppessoa carregarPessoas(char *file, plocal e, int nEspacos) {
    int i, num, haEspaco = 0;
    ppessoa novo, anterior = NULL, lista = NULL;
    pessoa aux;

    FILE *fr = fopen(file, "r");
    if (!fr) {
        printf("\a");
        printf("ERRO NO ACESSO AO FICHEIRO DE TEXTO [%s].\n", file);
        exit(EXIT_FAILURE);
    }

    aux.next = NULL;

    char id[25], estado[25];
    int idade, diasDoente;

    while (fscanf(fr, "%s %d %s", id, &idade, estado) != EOF) { //while (!feof(in_file)) -> Devolve valor != 0 se o indicador de final de ficheiro foi atingido
        if (strcmp(estado, "D") == 0) { // if Return value = 0 then it indicates str1 is equal to str2.
            fscanf(fr, "%d", &diasDoente);
        }
        /*while (fscanf(fr, "%s %d %s", aux.id, &aux.idade, aux.estado) != EOF) { //while (!feof(in_file)) -> Devolve valor != 0 se o indicador de final de ficheiro foi atingido
        if (strcmp(aux.estado, "D") == 0) { // if Return value = 0 then it indicates str1 is equal to str2.
            fscanf(fr, "%d", &aux.diasDoente);
        }*/

        //verifica dados
        //se passaram a validaçao continua
        //printf("%d\n", validaPessoa(id, idade, estado, diasDoente, lista));
        if (validaPessoa(id, idade, estado, diasDoente, lista)) {

            strcpy(aux.id, id);
            aux.idade = idade;
            strcpy(aux.estado, estado);
            aux.diasDoente = diasDoente;

            novo = (ppessoa) malloc(sizeof (pessoa));
            if (!novo) {
                printf("Erro a reservar memoria.\n");
                break;
            }

            do {
                num = intUniformRnd(0, nEspacos - 1);

                if (e[num].capacidade > 0) {
                    aux.local = &e[num];
                    e[num].capacidade--;
                    //printf("%d\n", e[num].capacidade);
                    *novo = aux;
                    //1ª iteração, a lista vai estar a NULL
                    if (lista == NULL)
                        lista = novo;
                    else
                        anterior->next = novo;
                    break;
                }

                int contaVazios = 0;

                for (i = 0; i < nEspacos; i++) {
                    if (e[i].capacidade > 0)
                        haEspaco = 1;
                    else
                        contaVazios++;
                }

                if (contaVazios == nEspacos) {
                    anterior->next = NULL;
                    break;

                }
            } while (haEspaco != 0);
            anterior = novo;
            diasDoente = 0, idade = 0;
        }
    }
    fclose(fr);
    return lista;
}

int validaPessoa(char* id, int idade, char* estado, int diasDoente, ppessoa p) {

    if (p == NULL) {
        //printf("1a vez\n");
    } else {
        while (p != NULL) {
            if (strcmp(p->id, id) == 0) {
                printf("\n\aID %s REPETIDO (NAO VAI ENTRAR NA SIM)\n", id);
                printf("CARREGA EM QQ TECLA PARA CONTINURAR!\n");
                getch();
                return 0;
            }
            p = p->next;
        }
    }

    /*printf("%s - ",id);
    printf("%d - ",idade);
    printf("%s - ",estado);
    printf("%d\n",diasDoente);*/
    if (idade < 1 || idade > 99) {
        printf("\a");
        printf("\n\aID %s COM IDADE INVALIDA! [%d] (NAO VAI ENTRAR NA SIM)\n", id, idade);
        printf("CARREGA EM QQ TECLA PARA CONTINURAR!\n");
        getch();
        //printf("\nIDADE\n");
        return 0;
    }
    if (strcmp(estado, "S") == 0 || strcmp(estado, "I") == 0 || strcmp(estado, "D") == 0) {
    } else {
        printf("\a");
        printf("\n\aID %s COM ESTADO INVALIDO! [%d] (NAO VAI ENTRAR NA SIM)\n", id, estado);
        printf("CARREGA EM QQ TECLA PARA CONTINURAR!\n");
        getch();
        //printf("\nS -> [%d]\n",strcmp("S",estado));
        //printf("D -> [%d]\n",strcmp("D",estado));
        //printf("I -> [%d]",strcmp("I",estado));
        return 0;
    }
    return 1;
}

void libertaListaPessoas(ppessoa lista) {
    ppessoa aux;

    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }

}



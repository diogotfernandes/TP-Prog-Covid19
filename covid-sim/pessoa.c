#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        if (aux->local != NULL)
            printf("LocalID -> \t%d\n\n", aux->local->id);
        else
            printf("Sem Local\n\n");
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
    }
    fclose(fr);
    return lista;
}

void libertaListaPessoas(ppessoa lista) {
    ppessoa aux;

    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }

}


//Calculo da Probabilidade de Recuperar em cada Iteração (Dia)
//Calculo da Probabilidade de Ficar Imune (depois de curada) em cada Iteração (Dia)

void modeloPropagacao(ppessoa lista, int dia) {

    while (lista != NULL) {

        //VERIFICA SE EXISTE ALGUÉM CURADO [C]
        //SE EXISTIR CURADOS, CALCULA A PROBABILIDADE DE FICAR IMUNE [L]
        if (strcmp(lista->estado, "C") == 0) {
            int probImune = 0;

            probImune = probEvento(0.2);

            if (probImune) {
                printf("[ProbImune] %s FICOU IMUNE!\n", lista->id);
                strcpy(lista->estado, "L");
            }


            //printf("Estado: %s\n",lista->estado);
        }

        //DURAÇÃO MÁXIMA DA INFEÇÃO
        //5+1DIA POR CADA DEZENA DE ANOS
        //p.ex: 25 ANOS -> 5 + 2 = 7 DIAS PARA FICAR CURADO
        if (strcmp(lista->estado, "D") == 0) {
            int diaRecuperacao = 0;
            diaRecuperacao = 5 + (lista->idade / 10);

            if (lista->diasDoente >= diaRecuperacao) {
                printf("\n%s FICOU CURADO APOS %d DIAS!\n", lista->id, diaRecuperacao);
                strcpy(lista->estado, "C");
            }
            printf("Nome: %s \t", lista->id);
            printf("DiasRecuperar [%d] - [%d] DiasDoente\n\n", lista->diasDoente, diaRecuperacao);
        }


        if (strcmp(lista->estado, "D") == 0) {
            int probRec = 0;
            float i = 0.0;

            //CALCULAR A PROBABILIDADE DE FICAR RECUPERADO
            i = (float) 1 / lista->idade;
            probRec = probEvento(i);

            //printf("[%s] - [%d]\n", lista->id, lista->idade);
            //printf("1/%d = %f\n", lista->idade, i);

            //CASO PROB = 1, O DOENTE FICA CURADO
            if (probRec) {
                printf("[ProbRecup] %s FICOU CURADO!\n", lista->id);
                strcpy(lista->estado, "C");
            }

        }

        lista->diasDoente++;
        lista = lista->next;
    }
}

void taxaDisseminacao(ppessoa lista, plocal e, int nEspacos) {
    int totalDoentes = 0, totalPessoas = 0;
    
    

    for (int i = 0; i < nEspacos; i++) {
        ppessoa p = lista;
        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
            }
            p = p->next;
        }
        printf("%d PESSOAS NA SALA ID[%d]\n", totalPessoas, e[i].id);
        totalPessoas = 0;
    }
}
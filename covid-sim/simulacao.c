#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"
#include "pessoa.h"
#include "utils.h"

#define PROBIMUNE 0.2



//Calculo da Probabilidade de Recuperar em cada Iteração (Dia)
//Calculo da Probabilidade de Ficar Imune (depois de curada) em cada Iteração (Dia)

void modeloPropagacao(ppessoa lista, int dia) {

    while (lista != NULL) {

        //VERIFICA SE EXISTE ALGUÉM CURADO [C]
        //SE EXISTIR CURADOS, CALCULA A PROBABILIDADE DE FICAR IMUNE [L]
        if (strcmp(lista->estado, "C") == 0) {
            int probImune = 0;

            probImune = probEvento(PROBIMUNE);
            if (probImune) {
                strcpy(lista->estado, "L");
                printf("[ProbImune] %s FICOU IMUNE! [%s]\n", lista->id, lista->estado);     
            }
        }
        
        //DURAÇÃO MÁXIMA DA INFEÇÃO
        //5+1DIA POR CADA DEZENA DE ANOS
        //p.ex: 25 ANOS -> 5 + 2 = 7 DIAS PARA FICAR CURADO
        if (strcmp(lista->estado, "D") == 0) {
            int diaRecuperacao = 0;
            diaRecuperacao = 5 + (lista->idade / 10);

            if (lista->diasDoente >= diaRecuperacao) {
                printf("%s FICOU CURADO APOS %d DIAS!\n", lista->id, diaRecuperacao);
                strcpy(lista->estado, "C");
            }
            printf("Nome: %s - ", lista->id);
            printf("DiasDoente [%d] - [%d] DiasRecuperar\n\n", lista->diasDoente, diaRecuperacao);
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
    int totalDoentes = 0, totalPessoas = 0, novosDoentes = 0;

    for (int i = 0; i < nEspacos; i++) {
        ppessoa p = lista;
        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
                //printf("%s\n", &p->id);
                if (strcmp(p->estado, "D") == 0) {
                    totalDoentes++;
                }
            }
            p = p->next;
        }
        
        novosDoentes = (int)(totalPessoas * 0.05) * totalDoentes;
        
        printf("NOVOS DOENTES: %d\n",novosDoentes);
        
        //printf("%d PESSOA(S) NA SALA ID[%d] \t [%d] PESSOA(S) DOENTE(S)\n", totalPessoas, e[i].id, totalDoentes);
        totalPessoas = 0;
        totalDoentes = 0;
    }
}

ppessoa doenteAleatorio(ppessoa p)
{
    
    ppessoa aux = p;
    ppessoa random = aux;
    
    int count = 2;
    
    while (aux->next != NULL) {
        if(rand() % count == (count - 1))
            random = aux->next;
        aux = aux->next;
        count++;
    }
    
    //printf("RANDOM ID: %s\n",random->id);
    
    return random;

    
    /*struct mynode *select_mynode(struct mylist *list) {
    struct mynode *list_iter = list->first; // An iterator to scan the list
    struct mynode *sel_node = list_iter; // The node that will be selected
     *
    int count = 2; // Temporary size of the list
    srand((unsigned int) time(NULL)); // Seed
    // Select a random element in O(n)
    while (list_iter->next != NULL) {
        if (rand() % count == (count - 1))
            sel_node = list_iter->next;
        list_iter = list_iter->next;
        count++;
    }
    return sel_node;
}*/
}

void pessoasLocal(ppessoa lista, plocal e, int nEspacos) {
    int totalDoentes = 0, totalPessoas = 0;

    for (int i = 0; i < nEspacos; i++) {
        ppessoa p = lista;
        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
                //printf("%s\n", &p->id);
                if (strcmp(p->estado, "D") == 0) {
                    totalDoentes++;
                }
            }
            p = p->next;
        }
        printf("%d PESSOA(S) NA SALA ID[%d] \t-\t [%d] PESSOA(S) DOENTE(S)\n", totalPessoas, e[i].id, totalDoentes);
        totalPessoas = 0;
        totalDoentes = 0;
    }
}

void escreveReport(ppessoa lista, plocal e, int nEspacos){
     FILE *fw;
    
    fw = fopen("report.txt","w");
    
    if (!fw) {
        printf("\a");
        printf("ERRO NO ACESSO AO FICHEIRO DE TEXTO [].\n");
        exit(EXIT_FAILURE);
    }
    
    int totalDoentes = 0, totalPessoas = 0;

    fprintf(fw,"--PESSOAS POR SALA--\n");
    for (int i = 0; i < nEspacos; i++) {
        ppessoa p = lista;
        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
                //printf("%s\n", &p->id);
                if (strcmp(p->estado, "D") == 0) {
                    totalDoentes++;
                }
            }
            p = p->next;
        }
        fprintf(fw,"%d PESSOA(S) NA SALA ID[%d] \t-\t [%d] PESSOA(S) DOENTE(S)\n", totalPessoas, e[i].id, totalDoentes);
        totalPessoas = 0;
        totalDoentes = 0;
    }
    fprintf(fw,"\n");
    fclose(fw);
}

void escreveFicheiroPop(ppessoa p, char *file)
{
    FILE *fw;
    
    fw = fopen(file,"w");
    
    if (!fw) {
        printf("\a");
        printf("ERRO NO ACESSO AO FICHEIRO DE TEXTO [].\n");
        exit(EXIT_FAILURE);
    }
    
    while (p != NULL) {
        fprintf(fw, "%s\t%d\t%s",p->id,p->idade,p->estado);
        if (strcmp(p->estado, "D") == 0){
            fprintf(fw,"\t%d",p->diasDoente);
        }
        fprintf(fw,"\n");
        p = p->next;
    }
    
    fclose(fw);
}
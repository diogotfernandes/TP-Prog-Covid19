#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "local.h"
#include "pessoa.h"
#include "utils.h"
#include "simulacao.h"

#define PROBIMUNE 0.2

//NOVA ITERAÇÃO NA SIMULAÇÃO
void simularDia(ppessoa p, int dia, int *n, plocal e) {

    //printf("\n*********************************************** DIA %d DA SIMULACAO! ***********************************************\n", dia);
    modeloPropagacao(p, dia);
    printf("\n********************************************* FIM DO DIA %d DA SIMULACAO! ******************************************\n", dia);
}
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
                strcpy(lista->estado, "I");
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
                printf("[DiasRec]%s FICOU CURADO APOS %d DIAS!\n", lista->id, diaRecuperacao);
                strcpy(lista->estado, "C");
            }
            //printf("SalaID[%d]  Nome: %s - ", lista->local->id, lista->id);
            //printf("DiasDoente [%d] - [%d] DiasRecuperar\n\n", lista->diasDoente, diaRecuperacao);
        }

        //PROBABILIDADE DE FICAR RECUPERADO (2%)
        if (strcmp(lista->estado, "D") == 0) {
            int probRec = 0;
            float i = 0.0;
            //CALCULAR A PROBABILIDADE DE FICAR RECUPERADO
            i = (float) 1 / lista->idade;
            probRec = probEvento(i);
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

//CALCULA TAXA DE DISSEMINAÇÃO - É APLICADA A CADA NOVA ITERAÇÃO 
void taxaDisseminacao(ppessoa lista, plocal e, int nEspacos) {
    int totalDoentes, totalPessoas, novosDoentes, totalSaudaveis;
    ppessoa aux = lista;
    ppessoa p = lista;

    for (int i = 0; i < nEspacos; i++) {
        totalDoentes = 0;
        totalPessoas = 0;
        novosDoentes = 0;
        totalSaudaveis = 0;

        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
                if (strcmp(p->estado, "D") == 0)
                    totalDoentes++;
                if (strcmp(p->estado, "S") == 0)
                    totalSaudaveis++;
                //if (strcmp(p->estado, "C") == 0)
                //totalSaudaveis++;
            }
            p = p->next;
        }

        p = lista;

        novosDoentes = (int) (totalPessoas * 0.05) * totalDoentes; //CALCULA QUANTAS PESSOAS SAO INFETADAS

        //printf("\nSALA: %d \tN CONTAGIOS: %d -\tN SAUDAVEIS(estado = 'S'): %d\n", i, novosDoentes, totalSaudaveis);

        while (1) {
            if (aux->next == NULL) //SE CHEGOU AO FIM DA LISTA, MAS AINDA HÁ PESSOAS P/ INFETAR, VOLTA AO INÍCIO
                aux = lista;
            if (totalSaudaveis == 0)
                break;
            if (novosDoentes == 0)
                break;

            ppessoa random = doenteAleatorio(lista);
            if (random->local->id == e[i].id) {
                if (strcmp(random->estado, "S") == 0) {
                    printf("[TaxaDiss]%s FICOU INFETADO - SALA ID[%d]\n", &random->id, random->local->id);
                    strcpy(random->estado, "D");
                    random->diasDoente = 0;
                    novosDoentes--;
                    totalSaudaveis--;
                }
            }
            random = NULL;
            aux = aux->next;

        }

        aux = lista;

        //printf("\n%d PESSOA(S) NA SALA ID[%d] \t [%d] PESSOA(S) DOENTE(S)\n", totalPessoas, e[i].id, totalDoentes);

    }
}

//VAI RETORNAR UM ELEMENTO ALEATÓRIO DA LISTA LIGADA
ppessoa doenteAleatorio(ppessoa p) {

    ppessoa aux = p;
    ppessoa random = aux;

    int count = 2;

    while (aux->next != NULL) {
        if (rand() % count == (count - 1))
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

//MOSTRA ESTATISTICAS DAS PESSOAS
void pessoasLocal(ppessoa lista, plocal e, int nEspacos) {
    int totalDoentes = 0, totalPessoas = 0, totalSaudaveis = 0,
            totalImunes = 0, totalCurados = 0;
    float taxaSaudaveis = 0.0, taxaInfetados = 0.0, taxaImunes = 0.0, taxaCurados = 0.0;

    for (int i = 0; i < nEspacos; i++) {
        ppessoa p = lista;
        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
                //printf("%s\n", &p->id);
                if (strcmp(p->estado, "D") == 0) {
                    totalDoentes++;
                }
                if (strcmp(p->estado, "S") == 0) {
                    totalSaudaveis++;
                }
                if (strcmp(p->estado, "C") == 0) {
                    totalCurados++;
                }
                if (strcmp(p->estado, "I") == 0) {
                    totalImunes++;
                }
            }
            p = p->next;
        }
        p = lista;
        printf("--------------------------------------------------SALA: [%d]---------------------------------------------------------\n", e[i].id);
        printf("%d PESSOA(S) NA SALA ID[%d]\n",
                totalPessoas, e[i].id);

        taxaSaudaveis = (float) totalSaudaveis / totalPessoas * 100;
        printf("\n\tSAUDAVEIS:  %d -> \t%.2f%%\n", totalSaudaveis, taxaSaudaveis);

        taxaInfetados = (float) totalDoentes / totalPessoas * 100;
        printf("\tINFETADOS:   %d -> \t%.2f%%\n", totalDoentes, taxaInfetados);

        taxaCurados = (float) totalCurados / totalPessoas * 100;
        printf("\tCURADOS:     %d -> \t%.2f%%\n", totalCurados, taxaCurados);

        taxaImunes = (float) totalImunes / totalPessoas * 100;
        printf("\tIMUNES:      %d -> \t%.2f%%\n\n", totalImunes, taxaImunes);

        printf("--------------------------------------------------------------------------------------------------------------------\n\n");

        totalPessoas = 0;
        totalDoentes = 0;
        totalSaudaveis = 0;
        totalCurados = 0;
        totalImunes = 0;
        taxaImunes = 0;
        taxaSaudaveis = 0;
        taxaInfetados = 0;
    }
}

//ADICIONA NOVO DOENTE
ppessoa novoDoente(ppessoa p, plocal e, int total) {
    char id[25];
    int idade, diasDoente, localID;
    ppessoa novo;

    novo = malloc(sizeof (pessoa));
    if (!novo) {
        printf("Erro a reservar memoria.\n");
        return p;
    } else {
        printf("\nID: ");
        scanf(" %s", &id);
        printf("IDADE: ");
        scanf("%d", &idade);
        printf("DIAS DOENTE: ");
        scanf("%d", &diasDoente);


        if (validaPessoa(id, idade, "D", diasDoente, p)) {
            strcpy(novo->id, id);
            novo->idade = idade;
            strcpy(novo->estado, "D");
            novo->diasDoente = diasDoente;

            int ok = 0;
            do {
                printf("LOCAL ID: ");
                scanf("%d", &localID);
                for (int i = 0; i < total; i++) {
                    if (localID == e[i].id) {
                        novo->local = &e[i];
                        ok = 1;
                        printf("\n\nPESSOA ADICIONADA COM SUCESSO!\n");
                        break;
                    } else {
                        printf("LOCAL INEXISTENTE! TENTE NOVAMENTE...\n");
                    }
                }
            } while (ok == 0);


            novo->next = p;
            p = novo;
        }
    }
    return p;
}

//ESCREVE FICHEIRO DE TEXTO "report.txt"
void escreveReport(ppessoa lista, plocal e, int nEspacos, int nDia) {
    FILE *fw;

    fw = fopen("report.txt", "w");

    if (!fw) {
        printf("\a");
        printf("ERRO NO ACESSO AO FICHEIRO DE TEXTO [].\n");
        exit(EXIT_FAILURE);
    }

    int totalDoentes = 0, totalPessoas = 0, totalSaudaveis = 0,
            totalImunes = 0, totalCurados = 0;
    float taxaSaudaveis = 0.0, taxaInfetados = 0.0, taxaImunes = 0.0, taxaCurados = 0.0;


    for (int i = 0; i < nEspacos; i++) {
        ppessoa p = lista;
        while (p != NULL) {
            if (p->local->id == e[i].id) {
                totalPessoas++;
                //printf("%s\n", &p->id);
                if (strcmp(p->estado, "D") == 0) {
                    totalDoentes++;
                }
                if (strcmp(p->estado, "S") == 0) {
                    totalSaudaveis++;
                }
                if (strcmp(p->estado, "C") == 0) {
                    totalCurados++;
                }
                if (strcmp(p->estado, "I") == 0) {
                    totalImunes++;
                }
            }
            p = p->next;
        }
        p = lista;
        fprintf(fw,"TOTAL DIA(S) DE SIMULAÇÃO: %d\n\n",nDia);
        fprintf(fw, "--------------------------------------------------SALA: [%d]---------------------------------------------------------\n", e[i].id);
        fprintf(fw, "%d PESSOA(S) NA SALA ID[%d]\n",
                totalPessoas, e[i].id);

        taxaSaudaveis = (float) totalSaudaveis / totalPessoas * 100;
        fprintf(fw, "\n\tSAUDAVEIS:  %d -> \t%.2f%%\n", totalSaudaveis, taxaSaudaveis);

        taxaInfetados = (float) totalDoentes / totalPessoas * 100;
        fprintf(fw, "\tINFETADOS:   %d -> \t%.2f%%\n", totalDoentes, taxaInfetados);

        taxaCurados = (float) totalCurados / totalPessoas * 100;
        fprintf(fw, "\tCURADOS:     %d -> \t%.2f%%\n", totalCurados, taxaCurados);

        taxaImunes = (float) totalImunes / totalPessoas * 100;
        fprintf(fw, "\tIMUNES:      %d -> \t%.2f%%\n\n", totalImunes, taxaImunes);

        fprintf(fw, "--------------------------------------------------------------------------------------------------------------------\n\n");

        totalPessoas, totalDoentes, totalSaudaveis, totalCurados, totalImunes;
        taxaImunes, taxaSaudaveis, taxaInfetados = 0;
    }
    fprintf(fw, "\n");
    fclose(fw);
}

//ESCREVE NUM FICHEIRO DE TEXTO A POPULAÇÃO FINAL DA SIMULAÇÃO
void escreveFicheiroPop(ppessoa p, char *file) {
    FILE *fw;

    fw = fopen(file, "w");

    if (!fw) {
        printf("\a");
        printf("ERRO NO ACESSO AO FICHEIRO DE TEXTO [].\n");
        exit(EXIT_FAILURE);
    }

    while (p != NULL) {
        fprintf(fw, "%s\t%d\t%s", p->id, p->idade, p->estado);
        if (strcmp(p->estado, "D") == 0) {
            fprintf(fw, "\t%d", p->diasDoente);
        }
        fprintf(fw, "\n");
        p = p->next;
    }

    fclose(fw);
}
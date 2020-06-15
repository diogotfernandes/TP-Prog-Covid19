#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#include "pessoa.h"
#include "utils.h"


/*ESTADO POSSÍVEIS
 * [S] -> Saudável
 * [D] -> Doente
 * [I] -> Imune (prob de ficar imune (20%; após ser curado))
 * 
 * [C] -> Curado (após n dias ou c/ prob recuperaçao)
 */

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

        //VALIDA DADOS DAS PESSOAS - SE NÃO PASSAR NA VALIDAÇÃO, ESSA PESSOA É EXCLUÍDA DA SIMULAÇÃO
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

//VALIDA PESSOA
//SE NAO PASSAR NA VALIDALÇÃO (->id repetido; ->idade [1 - 99]; ->estado inválido ), NÃO ENTRA NA SIMULAÇÃO
int validaPessoa(char* id, int idade, char* estado, int diasDoente, ppessoa p) {

    //1ª VEZ, OU SEJA, A LISTA ESTÁ VAZIA E NAO VERIFICA
    if (p == NULL) {
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
    if (idade < 1 || idade > 99) {
        printf("\a");
        printf("\n\aID %s COM IDADE INVALIDA! [%d] (NAO VAI ENTRAR NA SIM)\n", id, idade);
        printf("CARREGA EM QQ TECLA PARA CONTINURAR!\n");
        getch();
        return 0;
    }

    //25 anos
    //recupera = 5 + 2 = 7 dias
    //se diasDoente >= recupera , salta fora
    if (strcmp(estado, "D") == 0) {
        int recupera = 0;
        recupera = 5 + (idade / 10);

        if (diasDoente >= recupera) {
            printf("\a");
            printf("\n\aID %s COM DIAS DOENTE SUPERIOR AO PERMITIDO! [%d] [%d>=%d] (NAO VAI ENTRAR NA SIM)\n", id, diasDoente, diasDoente, recupera);
            printf("CARREGA EM QQ TECLA PARA CONTINURAR!\n");
            getch();
            return 0;
        }
    }


    if (strcmp(estado, "S") == 0 || strcmp(estado, "I") == 0 || strcmp(estado, "D") == 0) {
    } else {
        printf("\a");
        printf("\n\aID %s COM ESTADO INVALIDO! [%d] (NAO VAI ENTRAR NA SIM)\n", id, estado);
        printf("CARREGA EM QQ TECLA PARA CONTINURAR!\n");
        getch();
        return 0;
    }
    return 1;
}

//DESTRUIR ELEMENTOS DA LISTA    
void libertaListaPessoas(ppessoa lista) {
    ppessoa aux;

    while (lista != NULL) {
        aux = lista;
        lista = lista->next;
        free(aux);
    }

}



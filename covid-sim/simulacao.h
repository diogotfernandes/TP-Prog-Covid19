/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   simulacao.h
 * Author: dfern
 *
 * Created on 14 de Junho de 2020, 22:43
 */

#ifndef SIMULACAO_H
#define SIMULACAO_H

//Calculo da Probabilidade de Recuperar em cada Iteração (Dia)
//Calculo da Probabilidade de Ficar Imune (depois de curada) em cada Iteração (Dia)
void modeloPropagacao(ppessoa lista, int dia);

//APLICA TAXA DE DISSEMINAÇÃO A CADA ITERAÇÃO (DIA)
void taxaDisseminacao(ppessoa lista, plocal e, int nEspacos);

//MOSTRA PESSOAS POR LOCAL
//MOSTRA DOENTES POR LOCAL
void pessoasLocal(ppessoa lista, plocal e, int nEspacos);

//ESCREVE FICHEIRO COM A POPULAÇAO QUANDO TERMINA SIMULAÇÃO
void escreveFicheiroPop(ppessoa p, char *file);

//ESCRE FICHEIRO "report.txt" COM INFORMAÇÃO SOBRE O FINAL DA SIMULAÇÃO
void escreveReport(ppessoa lista, plocal e, int nEspacos);



//void doenteAleatorio(ppessoa p);
ppessoa doenteAleatorio(ppessoa p);

#endif /* SIMULACAO_H */


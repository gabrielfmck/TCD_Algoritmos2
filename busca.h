/*
 * busca.h - Algoritmos de busca
 */

#ifndef BUSCA_H
#define BUSCA_H

/* busca linear: olha elemento por elemento, funciona em qualquer vetor
   retorna o indice ou -1 se nao encontrar */
int busca_linear(int *vetor, int tamanho, int elemento);

/* busca binaria: so funciona com vetor ordenado
   retorna o indice ou -1 se nao encontrar */
int busca_binaria(int *vetor, int tamanho, int elemento);

#endif
/*
 * busca.c - Implementacao dos algoritmos de busca
 */

#include "busca.h"
#include <stdio.h>

/* percorre o vetor do inicio ao fim procurando o elemento */
int busca_linear(int *vetor, int tamanho, int elemento) {
  int i;

  for (i = 0; i < tamanho; i++) {
    if (vetor[i] == elemento) {
      return i; /* achou, retorna a posicao */
    }
  }

  return -1; /* nao achou */
}

/* divide o vetor ao meio repetidamente ate achar ou nao ter mais onde procurar */
int busca_binaria(int *vetor, int tamanho, int elemento) {
  int inicio = 0;
  int fim = tamanho - 1;
  int meio;

  while (inicio <= fim) {
    meio = (inicio + fim) / 2;

    if (vetor[meio] == elemento) {
      return meio;
    } else if (vetor[meio] < elemento) {
      inicio = meio + 1; /* elemento ta na metade direita */
    } else {
      fim = meio - 1;    /* elemento ta na metade esquerda */
    }
  }

  return -1;
}
/*
 * utils.c - Funcoes auxiliares compartilhadas entre os modulos
 */

#include "utils.h"
#include <stdlib.h>
#include <string.h>

/* aloca um novo vetor e copia o conteudo de origem para ele usando memcpy
   Retorna o ponteiro para a copia, ou NULL se a alocacao falhar
   O chamador é responsavel por dar free() na copia quando nao precisar mais. */
int *copiar_vetor(int *origem, int tamanho) {
  int *copia;

  if (tamanho <= 0) {
    return NULL;
  }

  copia = (int *)malloc(tamanho * sizeof(int));
  if (copia == NULL) {
    return NULL; /* sem memoria */
  }

  memcpy(copia, origem, tamanho * sizeof(int));
  return copia;
}

/* verifica se o vetor esta em ordem crescente nao-decrescente (a[i] <= a[i+1]).
   Retorna 1 se ordenado (ou vazio/unitario), 0 se encontrar algum par fora de ordem */
int verificar_ordenacao(int *vetor, int tamanho) {
  int i;

  if (vetor == NULL || tamanho <= 1) {
    return 1; /* vetor vazio ou com 1 elemento ja ta ordenado */
  }

  for (i = 0; i < tamanho - 1; i++) {
    if (vetor[i] > vetor[i + 1]) {
      return 0; /* achou o elemento fora de ordem */
    }
  }

  return 1;
}

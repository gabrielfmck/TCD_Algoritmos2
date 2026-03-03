/*
 * Implementacao de algumas funcoes auxiliares
 */

#include "utils.h"
#include <stdlib.h>
#include <string.h>

/* copia o vetor e retorna o ponteiro, NULL se der erro */
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

/* checa se o vetor ta em ordem crescente */
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

/*
 * busca.c - Implementacao dos algoritmos de busca
 *
 * Dois algoritmos com complexidades opostas:
 *  - Busca Linear:  O(n) - funciona em qualquer vetor
 *  - Busca Binaria: O(log n) - exige vetor ordenado
 */

#include "busca.h"
#include <stdio.h>

/*
 * Busca Linear - O(n)
 * Percorre o vetor sequencialmente comparando cada elemento com o alvo
 * Funciona em qualquer vetor (se tiver ordenado ou nao), retorna o indice ou -1
 */
int busca_linear(int *vetor, int tamanho, int elemento) {
  int i;

  for (i = 0; i < tamanho; i++) {
    if (vetor[i] == elemento) {
      return i; /* achou, retorna a posicao */
    }
  }

  return -1; /* nao achou */
}

/*
 * Busca Binaria - O(log n) [EXIGE VETOR ORDENADO]
 * A cada passo compara o alvo com o elemento do meio:
 *   - igual: achou, retorna o indice
 *   - menor: descarta a metade direita (inicio = meio + 1)
 *   - maior: descarta a metade esquerda (fim = meio - 1)
 * A cada comparacao o espaco de busca cai pela metade, dai O(log n).
 */
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
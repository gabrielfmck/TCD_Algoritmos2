/*
 * arquivo.c - Implementacao das funcoes de carregar e salvar arquivos
 */

#include "arquivo.h"
#include <stdio.h>
#include <stdlib.h>

/* le os numeros do arquivo e guarda no vetor, cresce o vetor com realloc */
int carregar_arquivo(char *nome_arquivo, int **vetor, int *tamanho) {
  FILE *arq;
  int *tmp;
  int numero;
  int cap = 10; /* comeca pequeno, vai crescendo conforme precisa */
  int n = 0;

  arq = fopen(nome_arquivo, "r");
  if (arq == NULL) {
    return -1;
  }

  *vetor = (int *)malloc(cap * sizeof(int));
  if (*vetor == NULL) {
    fclose(arq);
    return -1;
  }

  while (fscanf(arq, "%d", &numero) == 1) {
    /* se encheu o espaco, aumenta mais 10 */
    if (n == cap) {
      cap += 10;
      tmp = (int *)realloc(*vetor, cap * sizeof(int));
      if (tmp == NULL) {
        free(*vetor);
        *vetor = NULL;
        fclose(arq);
        return -1;
      }
      *vetor = tmp;
    }
    (*vetor)[n] = numero;
    n++;
  }

  fclose(arq);

  /* arquivo vazio nao tem utilidade */
  if (n == 0) {
    free(*vetor);
    *vetor = NULL;
    return -1;
  }

  *tamanho = n;
  return 0;
}

/* salva o vetor num arquivo txt, um numero por linha */
int salvar_vetor(char *nome_arquivo, int *vetor, int tamanho) {
  FILE *arq;
  int i;

  arq = fopen(nome_arquivo, "w");
  if (arq == NULL) {
    return -1;
  }

  for (i = 0; i < tamanho; i++) {
    fprintf(arq, "%d\n", vetor[i]);
  }

  fclose(arq);
  return 0;
}

/*
 * ordenacao.c - Implementacao dos algoritmos de ordenacao
 */

#include "ordenacao.h"
#include <stdio.h>
#include <stdlib.h>

/* pega cada elemento e insere ele na posicao certa entre os anteriores */
void insertion_sort(int *vetor, int tamanho) {
  int i, j, chave;

  for (i = 1; i < tamanho; i++) {
    chave = vetor[i];
    j = i - 1;

    /* empurra os elementos maiores que chave uma posicao pra frente */
    while (j >= 0 && vetor[j] > chave) {
      vetor[j + 1] = vetor[j];
      j--;
    }

    vetor[j + 1] = chave;
  }
}

/* compara pares vizinhos e vai jogando o maior pro final */
void bubble_sort(int *vetor, int tamanho) {
  int i, j, tmp;
  int trocou;

  for (i = 0; i < tamanho - 1; i++) {
    trocou = 0;

    for (j = 0; j < tamanho - 1 - i; j++) {
      if (vetor[j] > vetor[j + 1]) {
        tmp = vetor[j];
        vetor[j] = vetor[j + 1];
        vetor[j + 1] = tmp;
        trocou = 1;
      }
    }

    /* se nao trocou nada, ja ta ordenado */
    if (!trocou)
      break;
  }
}

/* acha o menor do restante e coloca na posicao correta */
void selection_sort(int *vetor, int tamanho) {
  int i, j, min, tmp;

  for (i = 0; i < tamanho - 1; i++) {
    min = i;

    for (j = i + 1; j < tamanho; j++) {
      if (vetor[j] < vetor[min]) {
        min = j;
      }
    }

    /* so troca se achou um menor */
    if (min != i) {
      tmp = vetor[i];
      vetor[i] = vetor[min];
      vetor[min] = tmp;
    }
  }
}

/* junta dois pedacos ordenados em um so */
void merge(int *vetor, int esq, int meio, int dir) {
  int i, j, k;
  int tam_esq = meio - esq + 1;
  int tam_dir = dir - meio;

  int *tmp_esq = (int *)malloc(tam_esq * sizeof(int));
  int *tmp_dir = (int *)malloc(tam_dir * sizeof(int));

  if (tmp_esq == NULL || tmp_dir == NULL) {
    free(tmp_esq);
    free(tmp_dir);
    return;
  }

  for (i = 0; i < tam_esq; i++)
    tmp_esq[i] = vetor[esq + i];
  for (j = 0; j < tam_dir; j++)
    tmp_dir[j] = vetor[meio + 1 + j];

  i = 0;
  j = 0;
  k = esq;

  /* intercala os dois pedacos em ordem */
  while (i < tam_esq && j < tam_dir) {
    if (tmp_esq[i] <= tmp_dir[j]) {
      vetor[k] = tmp_esq[i];
      i++;
    } else {
      vetor[k] = tmp_dir[j];
      j++;
    }
    k++;
  }

  /* copia o que sobrou */
  while (i < tam_esq) {
    vetor[k] = tmp_esq[i];
    i++;
    k++;
  }
  while (j < tam_dir) {
    vetor[k] = tmp_dir[j];
    j++;
    k++;
  }

  free(tmp_esq);
  free(tmp_dir);
}

/* funcao recursiva do merge sort */
void merge_sort_rec(int *vetor, int esq, int dir) {
  int meio;

  if (esq >= dir)
    return;

  meio = (esq + dir) / 2;

  merge_sort_rec(vetor, esq, meio);
  merge_sort_rec(vetor, meio + 1, dir);
  merge(vetor, esq, meio, dir);
}

/* divide o vetor ao meio, ordena cada metade e depois junta */
void merge_sort(int *vetor, int tamanho) {
  merge_sort_rec(vetor, 0, tamanho - 1);
}

/* coloca o pivo no lugar certo e retorna a posicao dele
   usa o elemento do meio como pivo pra nao ter problema com vetor ja ordenado */
int particionar(int *vetor, int esq, int dir) {
  int meio = esq + (dir - esq) / 2;
  int pivo, i, j, tmp;

  /* move o elemento do meio pro final pra usar como pivo */
  tmp = vetor[meio]; vetor[meio] = vetor[dir]; vetor[dir] = tmp;

  pivo = vetor[dir];
  i = esq - 1;

  for (j = esq; j < dir; j++) {
    if (vetor[j] <= pivo) {
      i++;
      tmp = vetor[i]; vetor[i] = vetor[j]; vetor[j] = tmp;
    }
  }

  tmp = vetor[i + 1]; vetor[i + 1] = vetor[dir]; vetor[dir] = tmp;

  return i + 1;
}

/* funcao recursiva do quick sort */
void quick_sort_rec(int *vetor, int esq, int dir) {
  int pos;

  if (esq >= dir)
    return;

  pos = particionar(vetor, esq, dir);

  quick_sort_rec(vetor, esq, pos - 1);
  quick_sort_rec(vetor, pos + 1, dir);
}

/* escolhe um pivo e divide em menores e maiores, repete recursivamente */
void quick_sort(int *vetor, int tamanho) {
  quick_sort_rec(vetor, 0, tamanho - 1);
}

/* implementar algoritmo extra aqui */
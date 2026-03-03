/*
 * log.c - Geracao do relatorio de desempenho
 */

#include "log.h"
#include "busca.h"
#include "ordenacao.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  /* para usar o QueryPerformanceCounter
                         clock() so tem precisao de ~1ms, o que nao e suficiente
                         para medir algoritmos rapidos nos vetores pequenos */

#define REPETICOES  100   /* repeticoes por algoritmo, a media aritmetica */
#define REP_BUSCA   10000  /* repeticoes para busca */

/* 100 repeticoes para ate 100k elementos.
   Para 1M nao repetimos porque Bubble/Insertion/Selection levariam horas */
int calc_reps(int tamanho) {
  if (tamanho <= 100000) return 100;
  return 1;
}

/* mede o tempo medio da busca linear, repetindo bastante pra ter precisao */
double medir_tempo_linear(int *vetor, int tamanho, int elemento) {
  int i;
  LARGE_INTEGER freq, inicio, fim;

  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < REP_BUSCA; i++) {
    busca_linear(vetor, tamanho, elemento);
  }
  QueryPerformanceCounter(&fim);

  return (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / REP_BUSCA;
}

/* mede o tempo medio da busca binaria (precisa de vetor ordenado) */
double medir_tempo_binaria(int *vetor_ord, int tamanho, int elemento) {
  int i;
  LARGE_INTEGER freq, inicio, fim;

  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < REP_BUSCA; i++) {
    busca_binaria(vetor_ord, tamanho, elemento);
  }
  QueryPerformanceCounter(&fim);

  return (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / REP_BUSCA;
}

/* executa cada algoritmo N vezes (100 para ate 100k, 1 para 1M), calcula a media aritmetica e salva */
void gerar_log(int *vetor, int tamanho, char *nome_arquivo) {
  FILE *arq;
  double t_insertion, t_bubble, t_selection, t_merge, t_quick;
  /* double t_extra; */ /* descomentar quando alg extra tiver implementado */
  double t_linear, t_binaria;
  int *vetor_ord, *copia;
  int elemento, pos_linear, pos_binaria;
  int reps, i;
  LARGE_INTEGER freq, inicio, fim;
  time_t agora;
  struct tm *tm_info;
  char data_hora[64];

  time(&agora);
  tm_info = localtime(&agora);
  strftime(data_hora, sizeof(data_hora), "%d/%m/%Y %H:%M:%S", tm_info);

  /* ordena uma copia para usar na busca binaria */
  vetor_ord = copiar_vetor(vetor, tamanho);
  if (vetor_ord == NULL) {
    printf("Erro ao alocar memoria para o log.\n");
    return;
  }
  quick_sort(vetor_ord, tamanho);

  /* usa o elemento do meio do vetor original como alvo da busca */
  elemento = vetor[tamanho / 2];
  /* pegamos a posicao real uma unica vez, fora do loop de medicao */
  pos_linear = busca_linear(vetor, tamanho, elemento);
  pos_binaria = busca_binaria(vetor_ord, tamanho, elemento);

  reps = calc_reps(tamanho);
  /* freq: quantos ticks por segundo o hardware conta (usado na conversao pro final) */
  QueryPerformanceFrequency(&freq);

  /* para cada algoritmo:
     - roda N vezes em uma copia do vetor original (importante: copia nova a cada vez,
       senao o vetor ja estaria ordenado da segunda execucao em diante)
     - calcula a media: (tempo_total / N) = tempo medio por execucao */
  printf("Medindo Insertion Sort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    insertion_sort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  /* (ticks_fim - ticks_inicio) / ticks_por_segundo / repeticoes = segundos por execucao */
  t_insertion = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;

  printf("Medindo Bubble Sort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    bubble_sort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  t_bubble = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;

  printf("Medindo Selection Sort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    selection_sort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  t_selection = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;

  printf("Medindo Merge Sort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    merge_sort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  t_merge = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;

  printf("Medindo Quick Sort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    quick_sort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  t_quick = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;

  /* descomentar quando o algoritmo extra tiver implementado
  printf("Medindo Algoritmo Extra...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    algoritmo_extra(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  t_extra = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;
  */

  /* mede as buscas */
  printf("Medindo Busca Linear...\n");
  t_linear = medir_tempo_linear(vetor, tamanho, elemento);

  printf("Medindo Busca Binaria...\n");
  t_binaria = medir_tempo_binaria(vetor_ord, tamanho, elemento);

  free(vetor_ord);

  arq = fopen(nome_arquivo, "w");
  if (arq == NULL) {
    printf("Erro ao criar %s\n", nome_arquivo);
    return;
  }

  fprintf(arq, "=== Relatorio de Desempenho ===\n");
  fprintf(arq, "Data/Hora         : %s\n", data_hora);
  fprintf(arq, "Tamanho do vetor  : %d elementos\n", tamanho);
  fprintf(arq, "Repeticoes        : %d execucoes por algoritmo (media aritmetica)\n", calc_reps(tamanho));
  fprintf(arq, "Elemento buscado  : %d\n\n", elemento);

  fprintf(arq, "-------------------------------\n");
  fprintf(arq, "Ordenacao (tempo medio de %d execucoes):\n", calc_reps(tamanho));
  fprintf(arq, "-------------------------------\n");
  fprintf(arq, "  Insertion Sort : %.9f s (%.4f ms)\n", t_insertion, t_insertion * 1000.0);
  fprintf(arq, "  Bubble Sort    : %.9f s (%.4f ms)\n", t_bubble, t_bubble * 1000.0);
  fprintf(arq, "  Selection Sort : %.9f s (%.4f ms)\n", t_selection, t_selection * 1000.0);
  fprintf(arq, "  Merge Sort     : %.9f s (%.4f ms)\n", t_merge, t_merge * 1000.0);
  fprintf(arq, "  Quick Sort     : %.9f s (%.4f ms)\n", t_quick, t_quick * 1000.0);
  /* fprintf(arq, "  Algoritmo Extra [EXTRA] : %.9f s (%.4f ms)\n", t_extra, t_extra * 1000.0); */ /* descomentar quando tiver implementado */

  fprintf(arq, "\n-------------------------------\n");
  fprintf(arq, "Busca (tempo medio, elemento: %d):\n", elemento);
  fprintf(arq, "-------------------------------\n");

  fprintf(arq, "  Busca Linear  : %.9f s (%.4f ms)  ->  ", t_linear, t_linear * 1000.0);
  if (pos_linear >= 0)
    fprintf(arq, "encontrado na posicao %d (vetor original)\n", pos_linear);
  else
    fprintf(arq, "nao encontrado\n");

  fprintf(arq, "  Busca Binaria : %.9f s (%.4f ms)  ->  ", t_binaria, t_binaria * 1000.0);
  if (pos_binaria >= 0)
    fprintf(arq, "encontrado na posicao %d (vetor ordenado)\n", pos_binaria);
  else
    fprintf(arq, "nao encontrado\n");

  /* descomentar quando algoritmo extra tiver implementado
  fprintf(arq, "\n===============================\n");
  fprintf(arq, "ALGORITMO EXTRA: (a definir)\n");
  fprintf(arq, "===============================\n");
  fprintf(arq, "  Origem:        (a preencher)\n");
  fprintf(arq, "  Justificativa: (a preencher)\n");
  fprintf(arq, "  Complexidade:  Melhor: O(?) | Medio: O(?) | Pior: O(?)\n");
  fprintf(arq, "  Resultado: %.9f s (%.4f ms)\n", t_extra, t_extra * 1000.0);
  fprintf(arq, "===============================\n");
  */

  fclose(arq);
}
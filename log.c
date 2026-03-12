/*
 * log.c - Geracao do relatorio de desempenho
 *
 * Mede o tempo medio de cada algoritmo de ordenacao e busca usando
 * o QueryPerformanceCounter (resolucao de nanosegundos). Cada algoritmo é
 * executado N vezes sobre uma copia nova do vetor original a cada rodada
 * (para nao medir o algoritmo num vetor ja ordenado) e dai o tempo total é
 * dividido por N para obter a media aritmetica
 */

#include "log.h"
#include "busca.h"
#include "ordenacao.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  /* QueryPerformanceCounter: timer de alta resolucao (de nanosegundos).
                         clock() da biblioteca padrao tem precisao de ~1ms, 
                         insuficiente para medir algoritmos rapidos se for em vetores pequenos. */

#define REPETICOES  100   /* repeticoes por algoritmo, a media aritmetica */
#define REP_BUSCA   10000  /* repeticoes pra busca */

/* numero de repeticoes por algoritmo a cada medicao, a media aritmetica
   suaviza variacao de medicao pelo sistema operacional */
int calc_reps(int tamanho) {
  /* para vetores grandes (> 100k), Bubble/Insertion/Selection levaria
     varios minutos com 100 repeticoes */
  if (tamanho <= 100000) return 100;
  return 1;
}

/* mede o tempo medio da busca linear repetindo REP_BUSCA vezes pra ter
   precisao suficiente, dai uma unica busca é rapida demais para medir sozinha */
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

/* mede o tempo medio da busca binaria (exige que o vetor ja esteja ordenado como entrada) */
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

/* executa cada algoritmo calc_reps(tamanho) vezes, calcula a media aritmetica
   dos tempos e salva o relatorio completo no arquivo que for indicado */
void gerar_log(int *vetor, int tamanho, char *nome_arquivo) {
  FILE *arq;
  double t_insertion, t_bubble, t_selection, t_merge, t_quick, t_dual;
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

  /* usa o elemento do meio do vetor original como alvo fixo da busca
     (escolha arbitraria, garante que o elemento existe no vetor) */
  elemento = vetor[tamanho / 2];
  /* posicoes reais obtidas uma unica vez, fora do loop de medicao,
     para nao contaminar o tempo medido com o custo da busca de referencia */
  pos_linear = busca_linear(vetor, tamanho, elemento);
  pos_binaria = busca_binaria(vetor_ord, tamanho, elemento);

  reps = calc_reps(tamanho);
  /* freq: quantos ticks por segundo o hardware conta (usado na conversao pro final) */
  QueryPerformanceFrequency(&freq);

  /* estrategia de medicao pra cada algoritmo:
     - cria uma copia nova do vetor a cada repeticao (importante: se reutilizassemos
       o mesmo vetor, a segunda execucao ja encontraria o vetor ordenado, dai medindo
       o melhor caso em vez do caso real da entrada)
     - divide o tempo total pelo numero de repeticoes para obter a media... */
  printf("Medindo Insertion Sort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    insertion_sort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  /* formula: (ticks_fim - ticks_inicio) / frequencia / repeticoes = segundos por execucao */
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

  printf("Medindo Introsort...\n");
  QueryPerformanceCounter(&inicio);
  for (i = 0; i < reps; i++) {
    copia = copiar_vetor(vetor, tamanho);
    introsort(copia, tamanho);
    free(copia);
  }
  QueryPerformanceCounter(&fim);
  t_dual = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / reps;

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
  fprintf(arq, "  Introsort [EXTRA] : %.9f s (%.4f ms)\n", t_dual, t_dual * 1000.0);

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

  fprintf(arq, "\n===============================\n");
  fprintf(arq, "ALGORITMO EXTRA: Introsort\n");
  fprintf(arq, "===============================\n");
  fprintf(arq, "  Origem:        Criado por David Musser (1997). Algoritmo hibrido\n");
  fprintf(arq, "                 que combina Quick Sort, Heap Sort e Insertion Sort.\n");
  fprintf(arq, "  Justificativa: O Quick Sort tem desempenho O(n^2) no pior caso\n");
  fprintf(arq, "                 (ex: vetor ja ordenado com pivo ruim). O Introsort\n");
  fprintf(arq, "                 monitora a profundidade de recursao e, ao ultrapassar\n");
  fprintf(arq, "                 2*log2(n), troca automaticamente para Heap Sort,\n");
  fprintf(arq, "                 garantindo O(n log n) mesmo no pior caso. Para\n");
  fprintf(arq, "                 subvetores pequenos, usa Insertion Sort que e mais\n");
  fprintf(arq, "                 eficiente em cache para poucos elementos.\n");
  fprintf(arq, "  Complexidade:  Melhor: O(n log n) | Medio: O(n log n) | Pior: O(n log n)\n");
  fprintf(arq, "  Resultado: %.9f s (%.4f ms)\n", t_dual, t_dual * 1000.0);
  fprintf(arq, "===============================\n");

  fclose(arq);
}
/*
 * main.c - Menu principal
 */

#include "arquivo.h"
#include "busca.h"
#include "log.h"
#include "ordenacao.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>  /* QueryPerformanceCounter para timer de alta resolucao */

/* limpa o lixo que fica no buffer apos o scanf */
void limpar_buffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

/* Submenu de busca */
void menu_busca(int *vetor, int tamanho) {
  int opcao, elemento;
  LARGE_INTEGER freq, inicio, fim;
  double tempo;

  QueryPerformanceFrequency(&freq);

  do {
    printf("\n=== Submenu de Busca ===\n");
    printf("1. Busca Linear\n");
    printf("2. Busca Binaria\n");
    printf("3. Voltar\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limpar_buffer();

    if (opcao == 3)
      break;

    if (opcao < 1 || opcao > 2) {
      printf("Opcao invalida.\n");
      continue;
    }

    printf("Digite o elemento a buscar: ");
    scanf("%d", &elemento);
    limpar_buffer();

    if (opcao == 1) {
      int i, pos;

      /* executa 10000 vezes para ter resolucao estatistica suficiente;
         operacoes rapidas demais seriam zero com apenas 1 medicao */
      QueryPerformanceCounter(&inicio);
      for (i = 0; i < 10000; i++)
        busca_linear(vetor, tamanho, elemento);
      QueryPerformanceCounter(&fim);
      tempo = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / 10000;

      pos = busca_linear(vetor, tamanho, elemento);
      if (pos >= 0)
        printf("Elemento %d encontrado na posicao %d.\n", elemento, pos);
      else
        printf("Elemento %d nao encontrado.\n", elemento);
      printf("Tempo: %.9f s\n", tempo);

    } else {
      int i, pos;
      char resposta;

      /* busca binaria exige vetor ordenado; se nao estiver, oferecer ordenacao */
      if (!verificar_ordenacao(vetor, tamanho)) {
        printf("O vetor nao esta ordenado.\n");
        printf("Deseja ordenar agora com Quick Sort para prosseguir? (s/n): ");
        scanf("%c", &resposta);
        limpar_buffer();
        if (resposta == 's' || resposta == 'S') {
          quick_sort(vetor, tamanho);
          printf("Vetor ordenado.\n");
        } else {
          printf("Busca Binaria cancelada.\n");
          continue;
        }
      }

      /* mesma logica: 10000 repeticoes para medir tempo medio com precisao */
      QueryPerformanceCounter(&inicio);
      for (i = 0; i < 10000; i++)
        busca_binaria(vetor, tamanho, elemento);
      QueryPerformanceCounter(&fim);
      tempo = (double)(fim.QuadPart - inicio.QuadPart) / freq.QuadPart / 10000;

      pos = busca_binaria(vetor, tamanho, elemento);
      if (pos >= 0)
        printf("Elemento %d encontrado na posicao %d.\n", elemento, pos);
      else
        printf("Elemento %d nao encontrado.\n", elemento);
      printf("Tempo: %.9f s\n", tempo);
    }

  } while (1);
}

/* Submenu de ordenacao */
void menu_ordenacao(int *vetor, int tamanho) {
  int opcao, k;
  int *copia; /* trabalha em cima da copia pra nao baguncar o original */
  char nome_arquivo[256];
  char resposta;

  do {
    printf("\n=== Submenu de Ordenacao ===\n");
    printf("1. Insertion Sort\n");
    printf("2. Bubble Sort\n");
    printf("3. Selection Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    /* printf("6. Algoritmo Extra\n"); */ /* descomentar quando algoritmo extra tiver implementado */
    printf("6. Voltar\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limpar_buffer();

    if (opcao == 6)
      break;

    if (opcao < 1 || opcao > 5) {
      printf("Opcao invalida.\n");
      continue;
    }

    copia = copiar_vetor(vetor, tamanho);
    if (copia == NULL) {
      printf("Erro ao copiar vetor.\n");
      continue;
    }

    switch (opcao) {
    case 1:
      printf("Executando Insertion Sort...\n");
      insertion_sort(copia, tamanho);
      break;
    case 2:
      printf("Executando Bubble Sort...\n");
      bubble_sort(copia, tamanho);
      break;
    case 3:
      printf("Executando Selection Sort...\n");
      selection_sort(copia, tamanho);
      break;
    case 4:
      printf("Executando Merge Sort...\n");
      merge_sort(copia, tamanho);
      break;
    case 5:
      printf("Executando Quick Sort...\n");
      quick_sort(copia, tamanho);
      break;
    /* descomentar quando algoritmo extra estiver implementado
    case 6:
      printf("Executando Algoritmo Extra...\n");
      algoritmo_extra(copia, tamanho);
      break;
    */
    }

    printf("Ordenacao concluida!\n");

    printf("Deseja salvar o vetor ordenado em um arquivo? (s/n): ");
    scanf("%c", &resposta);
    limpar_buffer();

    if (resposta == 's' || resposta == 'S') {
      printf("Digite o nome do arquivo de saida: ");
      scanf("%s", nome_arquivo);
      limpar_buffer();

      if (salvar_vetor(nome_arquivo, copia, tamanho) == 0)
        printf("Vetor salvo em '%s'.\n", nome_arquivo);
      else
        printf("Erro ao salvar o vetor.\n");
    }

    /* atualiza o vetor principal com o resultado ordenado */
    for (k = 0; k < tamanho; k++)
      vetor[k] = copia[k];
    free(copia);

  } while (1);
}

int main() {
  int *vetor = NULL;
  int tamanho = 0;
  int carregado = 0; /* 1 se ja carregou algum arquivo */
  int rodou = 0;     /* 1 se ja executou algum algoritmo */
  int opcao;
  char nome_arquivo[256];

  printf("==========================================\n");
  printf("   TCD - Algoritmos de Busca e Ordenacao  \n");
  printf("==========================================\n");

  do {
    printf("\n--- Menu Principal ---\n");
    printf("1. Carregar arquivo de dados\n");
    printf("2. Buscar elemento (linear ou binaria)\n");
    printf(
        "3. Ordenar dados (Insert., Bubble, Selection, Merge, Quick)\n");
    printf("4. Gerar relatorio (Log)\n");
    printf("5. Sair\n");
    printf("Escolha: ");
    scanf("%d", &opcao);
    limpar_buffer();

    switch (opcao) {
    case 1:
      /* se ja tinha algo carregado, libera antes de carregar outro */
      if (vetor != NULL) {
        free(vetor);
        vetor = NULL;
        tamanho = 0;
      }

      printf("Digite o nome do arquivo (na pasta corrente): ");
      scanf("%s", nome_arquivo);
      limpar_buffer();

      if (carregar_arquivo(nome_arquivo, &vetor, &tamanho) == 0) {
        printf("Arquivo '%s' carregado com sucesso! (%d elementos)\n",
               nome_arquivo, tamanho);
        carregado = 1;
        rodou = 0; /* novo arquivo: invalida qualquer execucao anterior */
      } else {
        printf("Erro ao carregar o arquivo '%s'.\n", nome_arquivo);
        carregado = 0;
      }
      break;

    case 2:
      if (!carregado) {
        printf("Carregue um arquivo primeiro (opcao 1).\n");
        break;
      }
      menu_busca(vetor, tamanho);
      rodou = 1; /* habilita geracao do log */
      break;

    case 3:
      if (!carregado) {
        printf("Carregue um arquivo primeiro (opcao 1).\n");
        break;
      }
      menu_ordenacao(vetor, tamanho);
      rodou = 1; /* habilita geracao do log */
      break;

    case 4:
      if (!rodou) {
        printf("Execute algum algoritmo antes de gerar o relatorio.\n");
        break;
      }
      /* roda cada algoritmo N vezes (100 para ate 100k, 1 para 1M) e salva a media */
      printf("Gerando relatorio (media aritmetica das execucoes)...\n");
      gerar_log(vetor, tamanho, "log.txt");
      printf("Relatorio salvo em 'log.txt'.\n");
      break;

    case 5:
      printf("Encerrando o programa...\n");
      break;

    default:
      printf("Opcao invalida. Tente novamente.\n");
      break;
    }

  } while (opcao != 5);

  /* libera a memoria antes de fechar */
  if (vetor != NULL) {
    free(vetor);
    vetor = NULL;
  }

  printf("Ate logo!\n");
  return 0;
}
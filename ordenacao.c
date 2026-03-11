/*
 * ordenacao.c - Implementacao dos algoritmos de ordenacao
 */

#include "ordenacao.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Insertion Sort - O(n^2) no pior e medio caso, O(n) no melhor (vetor ja ordenado)
 *
 * Ideia: mantém um subvetor da esquerda sempre ordenado, e a cada iteracao, pega
 * o proximo elemento (chave) e o move para a posicao correta dentro desse subvetor,
 * deslocando os elementos que sao maiores uma casa para a direita ate encontrar onde inserir.
 *
 * Uma analogia: é como ordenar cartas na mao, vc pega uma carta e a encaixa
 * na posicao certa entre as que ja estao ordenadas.
 *
 * Vantagem: eh excelente para vetores pequenos ou quase ordenados (com poucos deslocamentos).
 */
void insertion_sort(int *vetor, int tamanho) {
  int i, j, chave;

  for (i = 1; i < tamanho; i++) {
    chave = vetor[i]; /* elemento a ser inserido na posicao correta */
    j = i - 1;

    /* desloca os elementos maiores que chave uma posicao pra direita,
       abrindo espaco para inserir chave no lugar certo */
    while (j >= 0 && vetor[j] > chave) {
      vetor[j + 1] = vetor[j];
      j--;
    }

    vetor[j + 1] = chave; /* insere chave na posicao correta */
  }
}

/*
 * Bubble Sort - O(n^2) no pior e medio caso, O(n) no melhor (com otimizacao)
 *
 * Ideia dele: a cada passagem, compara pares de elementos vizinhos e troca eles se
 * estiverem fora de ordem, O maior elemento "borbulha" ate o final a cada volta,
 * por isso o laco interno pode encurtar (os ultimos i elementos ja estao no lugar).
 *
 * Otimizacao com a "trocou": se uma passagem completa nao realizou nenhuma
 * troca, o vetor ja esta ordenado e o algoritmo encerra de forma antecipada
 */
void bubble_sort(int *vetor, int tamanho) {
  int i, j, tmp;
  int trocou;

  for (i = 0; i < tamanho - 1; i++) {
    trocou = 0;

    /* compara pares vizinhos, apos cada passagem, o maior do trecho
       ja foi para o final, entao reduz o limite do laco interno */
    for (j = 0; j < tamanho - 1 - i; j++) {
      if (vetor[j] > vetor[j + 1]) {
        tmp = vetor[j];
        vetor[j] = vetor[j + 1];
        vetor[j + 1] = tmp;
        trocou = 1;
      }
    }

    /* se nao houve nenhuma troca, o vetor ja esta ordenado */
    if (!trocou)
      break;
  }
}

/*
 * Selection Sort - O(n^2) em todos os casos (nao e adaptativo)
 *
 * Ideia: divide o vetor em dois trechos, o da esquerda ja ordenado e o da
 * direita ainda nao ordenado, a cada passo, encontra o menor elemento do trecho
 * desordenado e troca ele com o primeiro elemento desse trecho, expandindo o lado
 * ordenado em um elemento por vez.
 *
 * Caracteristica dele: faz sempre n*(n-1)/2 comparacoes, mas no maximo n-1 trocas.
 * Mesmo que o vetor ja esteja ordenado, ele percorre tudo do mesmo jeito.
 */
void selection_sort(int *vetor, int tamanho) {
  int i, j, min, tmp;

  for (i = 0; i < tamanho - 1; i++) {
    min = i; /* assume que o menor do trecho desordenado esta na posicao i */

    /* busca o real menor elemento no trecho [i+1 .. tamanho-1] */
    for (j = i + 1; j < tamanho; j++) {
      if (vetor[j] < vetor[min]) {
        min = j;
      }
    }

    /* so troca se encontrou um elemento menor fora da posicao */
    if (min != i) {
      tmp = vetor[i];
      vetor[i] = vetor[min];
      vetor[min] = tmp;
    }
  }
}

/*
 * Funcao aux do Merge Sort: ele intercala dois subarrays adjacentes ja ordenados...
 * vetor[esq..meio] e vetor[meio+1..dir] sao copiados para buffers temporarios
 * e depois recolocados em ordem crescente de volta em vetor[esq..dir]
 * Usa a alocacao dinamica para os buffers, e libera no final.
 */
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

/* divide recursivamente ate subarrays de 1 elemento (caso base, ja ordenado),
   depois sobe na recursao unindo os pares com merge() */
void merge_sort_rec(int *vetor, int esq, int dir) {
  int meio;

  if (esq >= dir)
    return;

  meio = (esq + dir) / 2;

  merge_sort_rec(vetor, esq, meio);
  merge_sort_rec(vetor, meio + 1, dir);
  merge(vetor, esq, meio, dir);
}

/*
 * Merge Sort - O(n log n) em todos os casos, usa O(n) de memoria auxiliar
 *
 * Ideia do Merge: divisao e conquista, ele divide o vetor ao meio recursivamente ate ter
 * subarrays de 1 elemento (que ja estao ordenados), depois os
 * une dois a dois de forma ordenada, subindo na arvore de recursao.
 *
 * A vantagem dele: garante O(n log n) sempre, independente da entrada
 * A Desvantagem: precisa de memoria extra para os buffers temporarios do merge
 */
void merge_sort(int *vetor, int tamanho) {
  merge_sort_rec(vetor, 0, tamanho - 1);
}

/*
 * Funcao auxiliar do Quick Sort: rearranja vetor[esq..dir] em torno de um pivo.
 *
 * Escolha do pivo: usa o elemento do MEIO (movido pro final antes de particionar)
 * Isso evita o pior caso, O(n^2) que ocorreria em vetores ja ordenados se
 * a gente usar o primeiro ou ultimo elemento como pivo.
 *
 * Apos a execucao, todos os elementos a esquerda do pivo sao <= pivo,
 * e todos a direita sao > pivo, e retorna o indice final do pivo.
 */
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

/* aplica o particionamento recursivamente nas duas metades geradas pelo pivo */
void quick_sort_rec(int *vetor, int esq, int dir) {
  int pos;

  if (esq >= dir)
    return;

  pos = particionar(vetor, esq, dir);

  quick_sort_rec(vetor, esq, pos - 1);
  quick_sort_rec(vetor, pos + 1, dir);
}

/*
 * Quick Sort - O(n log n) no caso medio, O(n^2) no pior caso (evitado com pivo do meio)
 *
 * Ideia do Quick: escolhe um pivo, particiona o vetor em dois grupos (elementos menores
 * a esquerda e maiores a direita) e aplica o mesmo processo recursivamente
 * em cada grupo, o pivo do meio distribui melhor os elementos.
 *
 * Na pratica, é um dos algoritmos mais rapidos por ter boa localidade de cache
 * e baixa constante nas operacoes (poucas trocas, o acesso é sequencial a memoria).
 */
void quick_sort(int *vetor, int tamanho) {
  quick_sort_rec(vetor, 0, tamanho - 1);
}

/*
 * ================================================================
 * Algoritmo Extra: Introsort (David Musser, 1997)
 * ================================================================
 *
 * Algoritmo hibrido que combina tres algoritmos para garantir desempenho
 * otimo em qualquer situacao, sem os pontos fracos de cada um isolado:
 *
 *  1. Quick Sort    - usado no caso geral. Rapido na pratica (boa
 *                     localidade de cache), mas tem pior caso O(n^2).
 *
 *  2. Heap Sort     - ativado quando a recursao ultrapassa o limite
 *                     2*log2(n), indicando que o quick sort esta
 *                     degenerando. Garante O(n log n) no pior caso.
 *
 *  3. Insertion Sort - usado em subarrays com menos de LIMIAR elementos,
 *                     onde e mais eficiente que o quick sort pela
 *                     excelente localidade de cache em arrays pequenos
 *
 * Complexidade: O(n log n) garantido em todos os casos.
 * Curiosidade: É o algoritmo usado internamente no std::sort() do C++.
 * ================================================================
 */

/* subarrays com menos de LIMIAR_INSERCAO elementos sao finalizados
   com insertion sort, que supera o quick sort nessa escala pequena */
#define LIMIAR_INSERCAO 20

/*
 * Calcula o limite maximo de profundidade de recursao: 2 * floor(log2(n)).
 * Quando a recursao do quick sort ultrapassa esse limite, o pior caso O(n^2)
 * esta se manifestando - o introsort detecta isso e troca para heap sort.
 */
int calc_profundidade(int n) {
  int prof = 0;
  while (n > 1) { n /= 2; prof++; }
  return prof * 2;
}

/*
 * Sift-down: restaura a propriedade de max-heap a partir do no i
 * Compara o no i com seus filhos esquerdo (2i+1) e direito (2i+2)
 * Se algum filho for maior que o pai, troca com o maior e desce
 * recursivamente ate que a propriedade "pai >= filhos" esteja satisfeita
 */
void heapificar(int *vetor, int n, int i) {
  int maior = i;
  int esq = 2 * i + 1;
  int dir = 2 * i + 2;
  int tmp;

  if (esq < n && vetor[esq] > vetor[maior]) maior = esq;
  if (dir < n && vetor[dir] > vetor[maior]) maior = dir;

  if (maior != i) {
    tmp = vetor[i]; vetor[i] = vetor[maior]; vetor[maior] = tmp;
    heapificar(vetor, n, maior); /* desce recursivamente ate restaurar o heap */
  }
}

/*
 * Heap Sort aplicado apenas no trecho [esq..dir] do vetor.
 * Fase 1 - construcao do max-heap (de baixo para cima, O(n)):
 *   transforma o trecho em um heap onde o pai é sempre >= filhos.
 * Fase 2 - extracao ordenada (O(n log n)):
 *   Remove repetidamente o maior elemento (o raiz do heap) colocando ele
 *   no final do trecho e restaura a propriedade do heap no restante
 * Total: O(n log n) sem memoria extra, por isso é o fallback do introsort.
 */
void heap_sort_parcial(int *vetor, int esq, int dir) {
  int n = dir - esq + 1;
  int i, tmp;
  int *v = vetor + esq; /* ponteiro para o inicio do trecho a ordenar */

  /* fase 1: constroi o max-heap de baixo para cima */
  for (i = n / 2 - 1; i >= 0; i--)
    heapificar(v, n, i);

  /* fase 2: extrai o maior (raiz) e coloca no final, reduzindo o heap */
  for (i = n - 1; i > 0; i--) {
    tmp = v[0]; v[0] = v[i]; v[i] = tmp;
    heapificar(v, i, 0);
  }
}

/*
 * parte recursiva do Introsort, ela decide qual estrategia usar em cada chamada:
 *  - Trecho pequeno (< LIMIAR): insertion sort mais rapido nessa escala
 *  - Profundidade zerada (recursao muito funda): heap sort garante O(n log n)
 *  - Caso geral: quick sort com pivo no meio + duas chamadas recursivas
 */
void intro_rec(int *vetor, int esq, int dir, int prof) {
  int pivo, tmp, i, j;

  if (dir - esq < LIMIAR_INSERCAO) {
    /* trecho pequeno: insertion sort e mais eficiente aqui */
    int k, chave;
    for (k = esq + 1; k <= dir; k++) {
      chave = vetor[k];
      j = k - 1;
      while (j >= esq && vetor[j] > chave) {
        vetor[j + 1] = vetor[j];
        j--;
      }
      vetor[j + 1] = chave;
    }
    return;
  }

  if (prof == 0) {
    /* profundidade maxima atingida: o quick sort esta degenerando,
       usa heap sort para garantir O(n log n) */
    heap_sort_parcial(vetor, esq, dir);
    return;
  }

  /* caso geral: particiona com pivo no meio e ordena as duas metades */
  i = esq;
  j = dir;
  pivo = vetor[(esq + dir) / 2];

  while (i <= j) {
    while (vetor[i] < pivo) i++;
    while (vetor[j] > pivo) j--;
    if (i <= j) {
      tmp = vetor[i]; vetor[i] = vetor[j]; vetor[j] = tmp;
      i++; j--;
    }
  }

  intro_rec(vetor, esq, j,   prof - 1); /* metade esquerda */
  intro_rec(vetor, i,   dir, prof - 1); /* metade direita  */
}

/*
 * ponto de entrada do Introsort
 * Calcula o limite de profundidade (2*log2(n)) e inicia a recursao
 * Todo o trabalho e feito pelo intro_rec().
 */
void introsort(int *vetor, int tamanho) {
  if (tamanho <= 1) return;
  intro_rec(vetor, 0, tamanho - 1, calc_profundidade(tamanho));
}

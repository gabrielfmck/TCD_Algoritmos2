/*
 * ordenacao.h - Prototipos dos algoritmos de ordenacao
 *
 * Cinco algoritmos classicos + o algoritmo EXTRA (Introsort):
 *   Insertion Sort    - O(n^2) / O(n) melhor
 *   Bubble Sort       - O(n^2) / O(n) melhor
 *   Selection Sort    - O(n^2) sempre
 *   Merge Sort        - O(n log n) sempre, O(n) memoria extra
 *   Quick Sort        - O(n log n) medio, O(n^2) pior
 *   Introsort [EXTRA] - O(n log n) sempre (hibrido: Quick + Heap + Insertion)
 */

#ifndef ORDENACAO_H
#define ORDENACAO_H

void insertion_sort(int *vetor, int tamanho);
void bubble_sort(int *vetor, int tamanho);
void selection_sort(int *vetor, int tamanho);
void merge_sort(int *vetor, int tamanho);
void quick_sort(int *vetor, int tamanho);
void introsort(int *vetor, int tamanho); /* algoritmo extra */

#endif

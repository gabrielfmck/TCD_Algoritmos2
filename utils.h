/*
 * Funcoes auxiliares (tempo, verificacao, etc.)
 */

#ifndef UTILS_H
#define UTILS_H

/* copia o vetor, lembrar de dar free depois */
int *copiar_vetor(int *origem, int tamanho);

/* retorna 1 se estiver em ordem crescente, 0 se nao */
int verificar_ordenacao(int *vetor, int tamanho);

#endif

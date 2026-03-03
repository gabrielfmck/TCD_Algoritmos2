/*
 * Funcoes de carregar e salvar arquivos
 */

#ifndef ARQUIVO_H
#define ARQUIVO_H

/* le o arquivo e carrega os numeros no vetor, retorna 0 se ok */
int carregar_arquivo(char *nome_arquivo, int **vetor, int *tamanho);

/* salva o vetor num arquivo txt, retorna 0 se ok */
int salvar_vetor(char *nome_arquivo, int *vetor, int tamanho);

#endif
# TCD - Algoritmos de Busca e Ordenação

Trabalho de Conclusão de Disciplina - Algoritmos II

## Descrição

Programa em C que carrega uma lista de números inteiros de um arquivo de texto
e permite ao usuário realizar buscas (linear e binária) e ordenações com seis
algoritmos diferentes, além de gerar um relatório de desempenho com os tempos
médios de cada algoritmo.

## Arquivos do Projeto

```
TCD_Alg2/
├── main.c          -> menu principal e controle do programa
├── arquivo.c/.h    -> carrega e salva arquivos de dados
├── busca.c/.h      -> busca linear e binaria
├── ordenacao.c/.h  -> insertion, bubble, selection, merge, quick e introsort
├── log.c/.h        -> gera o relatorio log.txt
├── utils.c/.h      -> funcoes auxiliares (copiar vetor, verificar ordenacao)
├── dados.txt       -> arquivo de exemplo para testar o programa
├── log.txt         -> gerado automaticamente pelo programa (opcao 4)
└── README.md
```

## Como Compilar

Abra o terminal na pasta do projeto e rode:

**Linux / Mac:**
```
gcc -o tcd main.c arquivo.c busca.c ordenacao.c log.c utils.c -Wall
```

**Windows (CMD ou PowerShell com GCC instalado):**
```
gcc -o tcd.exe main.c arquivo.c busca.c ordenacao.c log.c utils.c -Wall
```

## Como Executar

**Linux / Mac:**
```
./tcd
```

**Windows:**
```
.\tcd.exe
```

## Como Usar

### Fluxo completo de uma sessão

O exemplo abaixo mostra uma sessão usando `dados.txt` (20 elementos), cobrindo
todas as funcionalidades do programa do início ao fim.

```
==========================================
   TCD - Algoritmos de Busca e Ordenacao
==========================================

--- Menu Principal ---
1. Carregar arquivo de dados
2. Buscar elemento (linear ou binaria)
3. Ordenar dados (Insert., Bubble, Selection, Merge, Quick, EXTRA)
4. Gerar relatorio (Log)
5. Sair
Escolha: 1
Digite o nome do arquivo (na pasta corrente): dados.txt
Arquivo 'dados.txt' carregado com sucesso! (20 elementos)

--- Menu Principal ---
...
Escolha: 2

=== Submenu de Busca ===
1. Busca Linear
2. Busca Binaria
3. Voltar
Escolha: 1
Digite o elemento a buscar: 42
Elemento 42 encontrado na posicao 0.
Tempo: 0.000000012 s

=== Submenu de Busca ===
1. Busca Linear
2. Busca Binaria
3. Voltar
Escolha: 2
Digite o elemento a buscar: 42
O vetor nao esta ordenado.
Deseja ordenar agora com Quick Sort para prosseguir? (s/n): s
Vetor ordenado.
Elemento 42 encontrado na posicao 10.
Tempo: 0.000000004 s

=== Submenu de Busca ===
1. Busca Linear
2. Busca Binaria
3. Voltar
Escolha: 3

--- Menu Principal ---
...
Escolha: 3

=== Submenu de Ordenacao ===
1. Insertion Sort
2. Bubble Sort
3. Selection Sort
4. Merge Sort
5. Quick Sort
6. Introsort (extra)
7. Voltar
Escolha: 6
Executando Introsort (extra)...
Ordenacao concluida!
Deseja salvar o vetor ordenado em um arquivo? (s/n): s
Digite o nome do arquivo de saida: saida_ordenada.txt
Vetor salvo em 'saida_ordenada.txt'.

=== Submenu de Ordenacao ===
...
Escolha: 7

--- Menu Principal ---
...
Escolha: 4
Gerando relatorio (media aritmetica das execucoes)...
Verificando corretude dos algoritmos...
Medindo Insertion Sort...
Medindo Bubble Sort...
Medindo Selection Sort...
Medindo Merge Sort...
Medindo Quick Sort...
Medindo Introsort...
Medindo Busca Linear...
Medindo Busca Binaria...
Relatorio salvo em 'log.txt'.

--- Menu Principal ---
...
Escolha: 5
Encerrando o programa...
Ate logo!
```

### Observações importantes

- **Opção 1** deve ser executada antes de qualquer outra operação.
- **Opção 4** só está disponível após executar pelo menos uma busca ou ordenação.
- **Opção 5** libera toda a memória alocada antes de encerrar.
- Se tentar usar busca binária em vetor desordenado, o programa oferece ordenar
  na hora. Se recusar, a busca é cancelada.
- Após cada ordenação, o programa pergunta se deseja salvar o vetor em arquivo.
- O arquivo `log.txt` é sobrescrito a cada vez que a opção 4 é executada.

### Detalhes de cada opção

**Opção 1 - Carregar arquivo de dados**
Informe o nome do arquivo (deve estar na mesma pasta do programa). Cada linha
do arquivo deve conter um número inteiro. O vetor é redimensionado dinamicamente
conforme o arquivo é lido.

**Opção 2 - Buscar elemento**
Submenu com Busca Linear (funciona em qualquer vetor) e Busca Binária (exige
vetor ordenado). O tempo é exibido com 9 casas decimais (precisão de nanosegundos).

**Opção 3 - Ordenar dados**
Submenu com os 6 algoritmos. Após ordenar, o vetor principal é atualizado.

**Opção 4 - Gerar relatório**
Executa cada algoritmo 100 vezes (para vetores até 100k elementos) e registra
a média aritmética em `log.txt`, junto com verificação de corretude e
justificativa do algoritmo extra.

**Opção 5 - Sair**
Libera toda a memória alocada e encerra.

## Formato do Arquivo de Entrada

Um número inteiro por linha, sem cabeçalho:

```
42
17
89
5
73
```

## Algoritmo Extra - Introsort

O algoritmo extra escolhido foi o **Introsort**.

Ele combina três algoritmos para garantir bom desempenho em qualquer situação:
- usa **Insertion Sort** para subarrays pequenos (menos de 16 elementos)
- usa **Quick Sort** com pivô no meio para o caso geral
- cai para **Heap Sort** se a recursão ficar muito funda (evita O(n²))

A profundidade máxima permitida é `2 * floor(log2(n))`. Por usar o elemento
do meio como pivô, ele se sai bem mesmo em vetores já ordenados, ao contrário
do Quick Sort clássico com pivô no final, que degenera nesses casos.
É o algoritmo que o C++ usa internamente no `std::sort()`.

## Relatório de Tempos (médias de 100 execuções)

Ambiente: preencher com CPU e SO usados nos testes
Arquivo de entrada: preencher com quantidade de elementos

| Algoritmo              | Tempo médio (s) |
|------------------------|-----------------|
| Insertion Sort         | (rodar e preencher) |
| Bubble Sort            | (rodar e preencher) |
| Selection Sort         | (rodar e preencher) |
| Merge Sort             | (rodar e preencher) |
| Quick Sort             | (rodar e preencher) |
| Introsort              | (rodar e preencher) |

> Os valores acima são gerados automaticamente pelo programa na opção 4
> e salvos no arquivo `log.txt`.

## Integrantes

- Gabriel Fernandes
- Davi Faria
- Ana Olívia
/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Implementação e avaliação de aplicações concorrentes (Parte 2) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../../lib/timer.h"

#define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor

float * alloc_array(float **, long long unsigned); // aloca o vetor
void init_arr(float *, long long unsigned); // inicializa o vetor aleatoriamente
void display_arr(float *, long long unsigned); // exibir vetor
void min_max_seq(float *, long long unsigned, float *, float *); // encontar menor e maior sequencial

int main(int argc, char * argv[]) {
    long long unsigned size;
    float * arr;
    float lesser, greater;
    double start_s, end_s, elapsed_s;

    // verificação de entradas
    switch (argc)
    {
        case 1:
            // Caso o programa seja chamado sem parametros, 
            // usarei um tamanho padrão para o vetor.
            size = DEFAULT_ARR_SIZE;
            break;
        case 2:
            // Caso o programa seja chamado com um parametro, 
            // é esperado que ele represente o tamanho do vetor.
            size = (long long unsigned) atoll(argv[1]);
            break;
        default:
            fprintf(stderr, "--ERRO: Numero de parametros incorreto!\n");
            fprintf(stderr, "Forma de uso: %s <array-size*>\n", argv[0]);
            fprintf(stderr, "Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // alocação do vetor
    alloc_array(&arr, size);

    // inicializando o vetor
    init_arr(arr, size);

    // exibindo o vetor
    // display_arr(arr, size);

    // menor e maior sequencial
    GET_TIME(start_s);
    min_max_seq(arr, size, &lesser, &greater);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("Menor: %3.2f\t\tMaior; %3.2f\n", lesser, greater);
    printf("Tempo usado pela funcao sequencial: %.6lf\n", elapsed_s);

    // liberando o vetor
    free(arr);

    return 0;
}

/* 
 * Esta função aloca o espaço necessário para um vetor.
 * Entradas esperadas:
 *   arr --> O ponteiro para uma variavel que guarda um ponteiro para float
 *   size --> O tamanho do vetor
 */
float * alloc_array(float ** arr, long long unsigned size) {
    *arr = malloc(sizeof(float) * size);
    if (!*arr) {
        printf("--ERRO: malloc()\n");
        exit(-2);
    }

    return *arr;
}

/* 
 * Esta função inicializa os elementos de um vetor de forma aleatória.
 * Entradas esperadas:
 *   arr --> Um ponteiro para float
 *   size --> O tamanho do vetor
 */
void init_arr(float * arr, long long unsigned size) {
    srand(time(NULL));

    while (size--) {
        *(arr + size) = (float) rand() / (float) (RAND_MAX / 100000.0);
    }
}

/* 
 * Esta função exibe um vetor.
 * Entradas esperadas:
 *   arr --> Ponteiro para float
 *   size --> O tamanho do vetor
 */
void display_arr(float * arr, long long unsigned size) {
    for (int i = 0; i < size; i++) {
        printf("%3.2f ", *(arr + i));
    }
    printf("\n");
}

/* 
 * Esta função encontra o menor e o maior valor dentro de um vetor.
 * Entradas esperadas:
 *   arr --> Ponteiro para float
 *   size --> O tamanho do vetor
 *   lesser --> Endereço para a variavel que guardará o menor
 *   greater --> Endereço para a variavel que guardará o maior
 */
void min_max_seq(float * arr, long long unsigned size, float * lesser, float * greater) {
    float less = *arr;
    float great = *arr;

    while (size--) {
        if (*(arr + size) > great)
            great = *(arr + size);
        if (*(arr + size) < less)
            less = *(arr + size);
    }

    *lesser = less;
    *greater = great;
}

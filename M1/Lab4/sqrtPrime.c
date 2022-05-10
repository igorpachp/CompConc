/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 4 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Comunicação e sincronização de threads com memória */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../../lib/timer.h"

#define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor
#define DEFAULT_NTHREADS 1 // número padrão de threads

typedef enum {False, True} boolean_t;

int nthreads;

boolean_t is_prime(int);
void init_arr(int *, long long unsigned); // inicializa o vetor aleatoriamente
void sqrt_primes_seq(int *, float *, long long unsigned); // calcula as raízes dos primos sequencialmente

int main(int argc, char * argv[]) {
    int * arr = NULL;
    long long unsigned size;

    // variaveis sequenciais
    double start_s, end_s, elapsed_s;
    float * roots_seq = NULL;

    // variaveis concorrentes
    float * roots_con = NULL;

    // verificação de entradas
    switch (argc)
    {
        case 1:
            // Caso o programa seja chamado sem parametros, 
            // usarei um tamanho padrão para o vetor e 
            // uma quantidade padrão de threads.
            size = DEFAULT_ARR_SIZE;
            nthreads = DEFAULT_NTHREADS;
            break;
        case 2:
            // Caso o programa seja chamado com um parametro, 
            // é esperado que ele represente o tamanho do vetor 
            // o número de threads será o padrão.
            size = (long long unsigned) atoll(argv[1]);
            nthreads = DEFAULT_NTHREADS;
            break;
        default:
            fprintf(stderr, "--ERRO: Numero de parametros incorreto!\n");
            fprintf(stderr, "Forma de uso: %s <array-size*>\n", argv[0]);
            fprintf(stderr, "Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // alocação de memória
    arr = malloc(sizeof(int) * size);
    if (!arr) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-2);
    }
    roots_seq = malloc(sizeof(float) * size);
    if (!roots_seq) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-2);
    }
    roots_con = malloc(sizeof(float) * size);
    if (!roots_con) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-2);
    }

    // inicializando o vetor
    init_arr(arr, size);

    // raiz sequencial
    GET_TIME(start_s);
    sqrt_primes_seq(arr, roots_seq, size);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;

    // exibindo vetores
    for (long long unsigned i = 0; i < size; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");
    for (long long unsigned i = 0; i < size; i++) {
        printf("%.2f ", *(roots_seq + i));
    }
    printf("\n");
    for (long long unsigned i = 0; i < size; i++) {
        printf("%.2f ", *(roots_con + i));
    }
    printf("\n");

    // medindo tempo
    printf("Tempo usado pela funcao sequencial: %.6lf\n", elapsed_s);

    // liberando memória
    free(arr);
    free(roots_seq);
    free(roots_con);

    return 0;
}

/* 
 * Esta função verifica se o dado número é primo.
 * Entradas esperadas:
 *   num --> Um número inteiro
 * Saída:
 *   True --> Caso seja primo.
 *   False --> Caso contrário.
 */
boolean_t is_prime(int num) {
    if (num <= 0 || num == 1) return False;

    int factor = 2;

    while (factor * factor < num) {
        if (num % factor++ == 0) return False;
    }

    return True;
}

/* 
 * Esta função procura por números primos em um 
 * vetor de inteiros e calcula suas raízes.
 * Entradas esperadas:
 *   num --> Um número inteiro
 * Saída:
 *   True --> Caso seja primo.
 *   False --> Caso contrário.
 */
void sqrt_primes_seq(int * arr, float * out, long long unsigned size) {
    while(size--) {
        if (is_prime(arr[size])) out[size] = sqrt(arr[size]);
        else out[size] = arr[size];
    }
}

/* 
 * Esta função inicializa os elementos do vetor de forma aleatória.
 * Entradas esperadas:
 *   arr --> Um vetor de inteiros.
 *   size --> O tamanho do vetor.
 */
void init_arr(int * arr, long long unsigned size) {
    srand(time(NULL));

    while (size--) {
        *(arr + size) = rand() % 101;
    }
}

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
#include <pthread.h>

#define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor
#define DEFAULT_NTHREADS 1 // número padrão de threads

typedef struct {
    long long unsigned id;
} minmax_args_t;

typedef struct {
    float lesser;
    float greater;
} minmax_ret_t;

long long unsigned arr_size;
unsigned nthreads;
float * arr_seq;
float * arr_con;

float * alloc_array(float **, long long unsigned); // aloca o vetor
void init_arr(float *, long long unsigned); // inicializa o vetor aleatoriamente
void display_arr(float *); // exibir vetor
void min_max_seq(float *, long long unsigned, float *, float *); // encontar menor e maior sequencial
void * min_max_con(void *); // encontar menor e maior concorrente

int main(int argc, char * argv[]) {
    pthread_t * tid_sys;

    // variaveis sequenciais
    float lesser, greater;
    double start_s, end_s, elapsed_s;

    // variaveis concorrentes
    minmax_args_t * args;
    minmax_ret_t * result;
    float min, max;
    double start_c, end_c, elapsed_c;

    // verificação de entradas
    switch (argc)
    {
        case 1:
            // Caso o programa seja chamado sem parametros, 
            // usarei um tamanho padrão para o vetor e 
            // uma quantidade padrão de threads.
            arr_size = DEFAULT_ARR_SIZE;
            nthreads = DEFAULT_NTHREADS;
            break;
        case 2:
            // Caso o programa seja chamado com um parametro, 
            // é esperado que ele represente o tamanho do vetor 
            // o número de threads será o padrão.
            arr_size = (long long unsigned) atoll(argv[1]);
            nthreads = DEFAULT_NTHREADS;
            break;
        case 3:
            // Receberemos o tamanho do vetor e o número de 
            // threads, nesta ordem.
            arr_size = (long long unsigned) atoll(argv[1]);
            nthreads = (unsigned) atoi(argv[2]);
            break;
        default:
            fprintf(stderr, "--ERRO: Numero de parametros incorreto!\n");
            fprintf(stderr, "Forma de uso: %s <array-size*> <number-of-threads>\n", argv[0]);
            fprintf(stderr, "Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // alocação de memória
    alloc_array(&arr_seq, arr_size);
    alloc_array(&arr_con, arr_size);

    // inicializando os vetores
    init_arr(arr_seq, arr_size);
    for (long long unsigned i; i < arr_size; i++)
        arr_con[i] = arr_seq[i];

    // menor e maior sequencial
    GET_TIME(start_s);
    min_max_seq(arr_seq, arr_size, &lesser, &greater);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("Menor: %3.2f\t\tMaior: %3.2f\n", lesser, greater);
    printf("Tempo usado pela funcao sequencial: %.6lf\n", elapsed_s);

    // menor e maior concorrente
    GET_TIME(start_c);
    tid_sys = malloc(sizeof(pthread_t) * nthreads);
    if (!tid_sys) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-2);
    }

    // criando threads
    for (long long unsigned i = 0; i < nthreads; i++) {
        args = (minmax_args_t *) malloc(sizeof(minmax_args_t));
        if (!args) {
            fprintf(stderr, "--ERRO: malloc()\n");
            exit(-2);
        }
        args->id = i;

        if (pthread_create(tid_sys + i, NULL, min_max_con, (void *) args)) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-3);
        };
    }

    max = min = *arr_con;
    // esperando threads
    for (int i = 0; i < nthreads; i++) {
        if(pthread_join(*(tid_sys + i), (void **) &result)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }

        // menor e maior globais
        max = result->greater > max ? result->greater : max;
        min = result->lesser < min ? result->lesser : min;
    }

    GET_TIME(end_c);
    elapsed_c = end_c - start_c;
    printf("Menor: %3.2f\t\tMaior: %3.2f\n", min, max);
    printf("Tempo usado pela funcao concorrente: %.6lf\n", elapsed_c);

    if (min == lesser && max == greater)
        printf("Os resultados são compativeis!\n");
    else
        printf("Os resultados são DIFERENTES!\n");

    // liberando memória
    free(arr_seq);
    free(arr_con);
    free(tid_sys);
    free(result);

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
void display_arr(float * arr) {
    for (int i = 0; i < arr_size; i++) {
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

/* 
 * Esta função encontra o menor e o maior valor dentro 
 * de um vetor de forma concorrente.
 * Entradas esperadas:
 *   id --> O id da thread
 * Saída:
 *   lesser --> Menor valor na seção do vetor
 *   greater --> Maior valor na seção do vetor
 */
void * min_max_con(void * arg) {
    minmax_args_t * args = (minmax_args_t *) arg;

    // as threads trabalham com blocos sequenciais de mesmo 
    // tamanho, a última thread recebe o resto quando houver
    float * ptr = arr_con + args->id * arr_size / nthreads;
    float * tail = (args->id == nthreads - 1) ? &arr_con[arr_size] : arr_con + (args->id+1) * arr_size / nthreads;

    minmax_ret_t * result = (minmax_ret_t *) malloc(sizeof(minmax_ret_t));
    if (!result) {
        printf("--ERRO: malloc()\n");
        exit(-2);
    }

    result->greater = result->lesser = *ptr;
    while (ptr != tail) {
        result->greater = *ptr > result->greater ? *ptr : result->greater;
        result->lesser = *ptr < result->lesser ? *ptr : result->lesser;
        ptr++;
    }

    free(args);

    return (void *) result;
}

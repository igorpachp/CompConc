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
#include <pthread.h>

#define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor
#define DEFAULT_NTHREADS 1 // número padrão de threads

typedef enum {False, True} boolean_t;

long long unsigned g_index = 0;
pthread_mutex_t mutex;

long long unsigned size;
int nthreads;

int * arr = NULL;
float * roots_seq = NULL;
float * roots_con = NULL;

boolean_t is_prime(int);
void init_arr(); // inicializa o vetor aleatoriamente
void sqrt_primes_seq(); // calcula as raízes dos primos sequencialmente
void * sqrt_primes_con(void *); // calcula as raízes dos primos de forma concorrente
boolean_t equals(); // verifica igualdade entre os elementos de dois vetores

int main(int argc, char * argv[]) {
    // variaveis sequenciais
    double start_s, end_s, elapsed_s;

    // variaveis concorrentes
    double start_c, end_c, elapsed_c;
    pthread_t * tid_sys;

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
        case 3:
            // Recebemos o tamanho do vetor e o número de 
            // threads, nesta ordem.
            size = (long long unsigned) atoll(argv[1]);
            nthreads = (unsigned) atoi(argv[2]);
            break;
        default:
            fprintf(stderr, "--ERRO: Numero de parametros incorreto!\n");
            fprintf(stderr, "Forma de uso: %s <array-size*> <number-of-threads*>\n", argv[0]);
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
    init_arr();

    // raiz sequencial
    GET_TIME(start_s);
    sqrt_primes_seq();
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;

    // raiz concorrente
    GET_TIME(start_c);
    pthread_mutex_init(&mutex, NULL);

    tid_sys = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (!tid_sys) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-2);
    }

    // criando threads
    for (int i = 0; i < nthreads; i++) {
        if (pthread_create(tid_sys + i, NULL, sqrt_primes_con, NULL)) {
			printf("--ERRO: pthread_create()\n");
            exit(-3);
        }
    }

    // esperando as threads
    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid_sys + i), NULL);
    }

    pthread_mutex_destroy(&mutex);
    GET_TIME(end_c);
    elapsed_c = end_c - start_c;

    // medindo tempo
    printf("Tempo usado pela funcao sequencial: %lf\n", elapsed_s);
    printf("Tempo usado pela funcao concorrente: %lf\n", elapsed_c);
    printf("Aceleração: %lf\n", elapsed_s / elapsed_c);

    // comparando resultados
    if (equals())
        printf("Os vetores resultantes são compativeis!\n");
    else
        printf("Os vetores resultantes são DIFERENTES!\n");

    // liberando memória
    free(arr);
    free(roots_seq);
    free(roots_con);
    free(tid_sys);

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
    if (num == 2 || num == 3) return True;
    if (num % 2 == 0) return False;

    int factor = 3;

    while (factor * factor <= num) {
        if (num % factor++ == 0) return False;
    }

    return True;
}

/* 
 * Esta função procura por números primos em um 
 * vetor de inteiros e calcula suas raízes 
 * de forma sequencial.
 */
void sqrt_primes_seq() {
    for(long long unsigned i = 0; i < size; i++) {
        roots_seq[i] = is_prime(arr[i]) ? sqrt(arr[i]) : arr[i];
    }
}

/* 
 * Esta função inicializa os elementos do vetor de forma aleatória.
 */
void init_arr() {
    srand(time(NULL));

    for (long long unsigned i = 0; i < size; i++) {
        *(arr + i) = rand() % (size + 1);
    }
}

/* 
 * Esta função procura por números primos em um 
 * vetor de inteiros e calcula suas raízes 
 * de forma concorrente.
 */
void * sqrt_primes_con(void * arg) {
    do {
        pthread_mutex_lock(&mutex);
        long long unsigned i = g_index++;
        pthread_mutex_unlock(&mutex);
        roots_con[i] = is_prime(arr[i]) ? sqrt(arr[i]) : arr[i];
    } while(g_index < size);

    pthread_exit(NULL);
}

/* 
 * Esta função compara os elementos de dois vetores de 
 * pontos flutuantes retornando se são iguais ou não.
 * Retorna:
 *   False --> em caso de haver algum elemento diferente
 *   True --> caso todos os elementos sejam iguais
 */
boolean_t equals() {
    for (long long unsigned i = 0; i < size; i++) {
        if (roots_con[i] != roots_seq[i]) return False;
    }

    return True;
}

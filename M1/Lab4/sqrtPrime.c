/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 4 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Comunicação e sincronização de threads com memória */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor
#define DEFAULT_NTHREADS 1 // número padrão de threads

typedef enum {False, True} boolean_t;

int * arr = NULL;
float * roots_seq = NULL;
float * roots_con = NULL;
long long unsigned size;
int nthreads;

boolean_t is_prime(int);
void init_arr(); // inicializa o vetor aleatoriamente
void display_arr(); // exibir vetor

int main(int argc, char * argv[]) {

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

    // inicializando o vetor
    init_arr();

    // exibindo vetor
    display_arr();
    for (long long unsigned i = 0; i < size; i++) {
        printf("%s ", is_prime(*(arr + i)) ? "sim" : "nao");
    }
    printf("\n");

    // liberando memória
    free(arr);

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
 * Esta função inicializa os elementos do vetor de forma aleatória.
 */
void init_arr() {
    long long unsigned arr_size = size;
    srand(time(NULL));

    while (arr_size--) {
        *(arr + arr_size) = rand() % 101;
    }
}

/* 
 * Esta função exibe um vetor.
 * Entradas esperadas:
 *   arr --> Ponteiro para float
 *   size --> O tamanho do vetor
 */
void display_arr() {
    for (long long unsigned i = 0; i < size; i++) {
        printf("%d ", *(arr + i));
    }
    printf("\n");
}

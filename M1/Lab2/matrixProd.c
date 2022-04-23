/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Implementação e avaliação de aplicações concorrentes */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define DEFAULT_M_SIZE   4 // tamanho padrão das matrizes
#define DEFAULT_NTHREADS 4 // número padão de threads

/* 
 * Estrutura para agrupar os argumentos esperados
 * pela função 'product'
 */
typedef struct {
	int row;
	int size;
} product_args_t;

float * a_matrix;
float * b_matrix;
float * res_matrix_seq; // matriz de resultado sequencial
float * res_matrix; // matriz de resultado sequencial

float * alloc_matrix(float **, unsigned); // alocar matrizes
void display_matrix(float *, unsigned); // exibir matrizes
void seq_product(unsigned); // produto sequencial
void * product(void *); // produto concorrente

int main(int argc, char * argv[]) {
    pthread_t * tid;
    product_args_t * args;
    unsigned nthreads;
    unsigned size;

    // verificação de entradas
    switch (argc)
    {
        case 1:
            // Caso o programa seja chamado sem parametros, 
            // usarei um tamanho padrão paras as matrizes 
            // e um número padrão de threads.
            size = DEFAULT_M_SIZE;
            nthreads = DEFAULT_NTHREADS;
            puts("1");
            break;
        case 2:
            // Caso o programa seja chamado com apenas um 
            // parametro, ele será o tamanho das as matrizes 
            // e usarei um número padrão de threads.
            size = atoi(argv[1]);
            nthreads = DEFAULT_NTHREADS;
            puts("2");
            break;
        case 3:
            // Caso o programa seja chamado os dois argumentos  
            // o primeiro sera o tamanho das as matrizes, e o 
            // segundo o número de threads.
            size = atoi(argv[1]);
            nthreads = atoi(argv[2]);
            puts("3");
            break;
        default:
            puts("ERRO: Numero de parametros incorreto!\n");
            puts("Forma de uso: ./<app> <matrix-size*> <number-of-threads*>\n");
            puts("Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // alocando memória para as matrizes
    alloc_matrix(&a_matrix, DEFAULT_M_SIZE);
    alloc_matrix(&b_matrix, DEFAULT_M_SIZE);
    alloc_matrix(&res_matrix_seq, DEFAULT_M_SIZE);
    alloc_matrix(&res_matrix, DEFAULT_M_SIZE);

    // inicialização
    srand(time(NULL));
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            *(a_matrix + row * size + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(b_matrix + row * size + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(res_matrix_seq + row * size + col) = 0.0;
            *(res_matrix + row * size + col) = 0.0;
        }
    }

    // produto
    seq_product(size);

    args = malloc(sizeof(product_args_t));
    if (!args) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    args->row = 0;
    args->size = size;
    product(args);

    // exibindo matrizes
    puts("matriz a");
    display_matrix(a_matrix, size);
    puts("\nmatriz b");
    display_matrix(b_matrix, size);
    puts("\nmatriz resultado sequencial");
    display_matrix(res_matrix_seq, size);
    puts("\nmatriz resultado concorrente");
    display_matrix(res_matrix, size);

    free(a_matrix);
    free(b_matrix);
    free(res_matrix_seq);
    free(res_matrix);

    return 0;
}

/* 
 * Esta função aloca o espaço necessário para uma matriz.
 * Entradas esperadas:
 *   M --> O ponteiro para uma variavel que guarda um ponteiro para float
 *   size --> O tamanho da matrize
 */
float * alloc_matrix(float ** M, unsigned size) {
    *M = malloc(sizeof(float) * size * size);
    if (!*M) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }

    return *M;
}

/* 
 * Esta função exibe uma matriz.
 * Entradas esperadas:
 *   M --> O ponteiro para uma matriz
 *   size --> O tamanho da matrize
 */
void display_matrix(float * M, unsigned size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%3.2f ", *(M + row * size + col));
        }
        printf("\n");
    }
}

/* 
 * Esta função calcula o produto entre duas matrizes de 
 * forma sequencial, salvando o resultado em uma terceira. 
 * É esperado que as matrizes fornecidas sejam quadradas.
 * Entradas esperadas:
 *   size --> O tamanho das matrizes
 */
void seq_product(unsigned size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            float sum = 0.0;
            for (int k = 0; k < size; k++) {
                sum += *(a_matrix + i*size + k) * *(b_matrix + k*size + j);
            }
            *(res_matrix_seq + i*size + j) = sum;
        }
    }
}

/* 
 * Esta função calcula o produto entre duas matrizes de 
 * forma concorrente, salvando o resultado em uma terceira 
 * matriz. É esperado que as matrizes fornecidas sejam quadradas.
 * Entradas esperadas:
 *   size --> O tamanho das matrizes
 */
void * product(void * arg) {
    product_args_t * args = (product_args_t *) arg;

    for (int i = 0; i < args->size; i++) {
        float sum = 0;
        for (int j = 0; j < args->size; j++) {
            sum += *(a_matrix + args->row*args->size + j) * *(b_matrix + j*args->size + i);
        }
        *(res_matrix + args->row*args->size + i) = sum;
    }

    free(arg);

    return NULL;
}

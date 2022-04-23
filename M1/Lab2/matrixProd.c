/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Implementação e avaliação de aplicações concorrentes */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M_SIZE 4

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

float * alloc_matrix(float **, unsigned);
void display_matrix(float *, unsigned);
void seq_product(unsigned);
void * product(void *);

int main(void) {
    product_args_t * arg;

    // alocando memória para as matrizes
    alloc_matrix(&a_matrix, M_SIZE);
    alloc_matrix(&b_matrix, M_SIZE);
    alloc_matrix(&res_matrix_seq, M_SIZE);
    alloc_matrix(&res_matrix, M_SIZE);

    // inicialização
    srand(time(NULL));
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            *(a_matrix + row * M_SIZE + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(b_matrix + row * M_SIZE + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(res_matrix_seq + row * M_SIZE + col) = 0.0;
            *(res_matrix + row * M_SIZE + col) = 0.0;
        }
    }

    // produto
    seq_product(M_SIZE);

    arg = malloc(sizeof(product_args_t));
    if (!arg) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    arg->row = 0;
    arg->size = M_SIZE;
    product(arg);

    // exibindo matrizes
    puts("matriz a");
    display_matrix(a_matrix, M_SIZE);
    puts("\nmatriz b");
    display_matrix(b_matrix, M_SIZE);
    puts("\nmatriz resultado sequencial");
    display_matrix(res_matrix_seq, M_SIZE);
    puts("\nmatriz resultado concorrente");
    display_matrix(res_matrix, M_SIZE);

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
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            printf("%3.2f ", *(M + row * M_SIZE + col));
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
    for (int i = 0; i < M_SIZE; i++) {
        for (int j = 0; j < M_SIZE; j++) {
            float sum = 0.0;
            for (int k = 0; k < M_SIZE; k++) {
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

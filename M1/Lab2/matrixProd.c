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

float * alloc_matrix(float **, unsigned);
void display_matrix(float *, unsigned);
void seq_product(float *, float *, float *, unsigned);

int main(void) {
    float * a_matrix;
    float * b_matrix;
    float * res_matrix; // matriz de resultado

    // alocando memória para as matrizes
    alloc_matrix(&a_matrix, M_SIZE);
    alloc_matrix(&b_matrix, M_SIZE);
    alloc_matrix(&res_matrix, M_SIZE);

    // inicialização
    srand(time(NULL));
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            *(a_matrix + row * M_SIZE + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(b_matrix + row * M_SIZE + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(res_matrix + row * M_SIZE + col) = 0.0;
        }
    }

    // produto
    seq_product(a_matrix, b_matrix, res_matrix, M_SIZE);

    // exibindo matrizes
    puts("matriz a");
    display_matrix(a_matrix, M_SIZE);
    puts("\nmatriz b");
    display_matrix(b_matrix, M_SIZE);
    puts("\nmatriz resultado");
    display_matrix(res_matrix, M_SIZE);

    free(a_matrix);
    free(b_matrix);
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
 * forma sequencial, salvando o resultado em uma terceira 
 * matriz também recebida na chamada. É esperado que as 
 * matrizes fornecidas sejam quadradas.
 * Entradas esperadas:
 *   A --> O ponteiro para uma matriz de números de ponto flutuante
 *   B --> O ponteiro para uma matriz de números de ponto flutuante
 *   C --> O ponteiro para a matriz de resultado
 *   size --> O tamanho das matrizes
 */
void seq_product(float * A, float * B, float * C, unsigned size) {

    for (int i = 0; i < M_SIZE; i++) {
        for (int j = 0; j < M_SIZE; j++) {
            float sum = 0.0;
            for (int k = 0; k < M_SIZE; k++) {
                sum += *(A + i*size + k) * *(B + k*size + j);
            }
            *(C + i*size + j) = sum;
        }
    }
}

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

int main(void) {
    float * a_matrix;
    float * b_matrix;
    float * res_matrix;

    // alocando memória para as matrizes
    a_matrix = malloc(sizeof(int) * M_SIZE * M_SIZE);
    if (!a_matrix) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    b_matrix = malloc(sizeof(int) * M_SIZE * M_SIZE);
    if (!b_matrix) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }
    res_matrix = malloc(sizeof(int) * M_SIZE * M_SIZE);
    if (!res_matrix) {
        printf("--ERRO: malloc()\n");
        exit(-1);
    }

    // inicialização
    srand(time(NULL));
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            *(a_matrix + row * M_SIZE + col) = (float) rand() / (float) (RAND_MAX / 100.0);
            *(b_matrix + row * M_SIZE + col) = (float) rand() / (float) (RAND_MAX / 100.0);
            *(res_matrix + row * M_SIZE + col) = 0.0;
        }
    }

    // exibindo matrizes
    puts("matriz a");
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            printf("%3.2f ", *(a_matrix + row * M_SIZE + col));
        }
        printf("\n");
    }
    puts("\nmatriz b");
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            printf("%.2f ", *(b_matrix + row * M_SIZE + col));
        }
        printf("\n");
    }
    puts("\nmatriz resultado");
    for (int row = 0; row < M_SIZE; row++) {
        for (int col = 0; col < M_SIZE; col++) {
            printf("%.2f ", *(res_matrix + row * M_SIZE + col));
        }
        printf("\n");
    }

    free(a_matrix);
    free(b_matrix);

    return 0;
}

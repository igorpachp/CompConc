/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Implementação e avaliação de aplicações concorrentes (Parte 2) */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor

float * alloc_array(float **, unsigned); // aloca o vetor
void init_arr(float *, unsigned); // inicializa o vetor aleatoriamente
void display_arr(float *, unsigned); // exibir vetor

int main(int argc, char * argv[]) {
    unsigned size;
    float * arr;

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
            size = atoi(argv[1]);
            break;
        default:
            puts("--ERRO: Numero de parametros incorreto!\n");
            puts("Forma de uso: ./<app> <array-size*>\n");
            puts("Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // alocação do vetor
    alloc_array(&arr, size);

    // inicializando o vetor
    init_arr(arr, size);

    // exibindo o vetor
    display_arr(arr, size);

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
float * alloc_array(float ** arr, unsigned size) {
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
void init_arr(float * arr, unsigned size) {
    srand(time(NULL));

    while (size--) {
        *(arr + size) = (float) rand() / (float) (RAND_MAX / 100.0);
    }
}

/* 
 * Esta função exibe um vetor.
 * Entradas esperadas:
 *   arr --> Ponteiro para float
 *   size --> O tamanho do vetor
 */
void display_arr(float * arr, unsigned size) {
    for (int i = 0; i < size; i++) {
        printf("%3.2f ", *(arr + i));
    }
    printf("\n");
}

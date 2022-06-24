#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "concorrente.h"

/* 
 * Esta função utiliza o método dos trapézios para calcular a integral 
 * de uma função discreta à partir dos vetores de coordenadas de forma 
 * concorrente, fica como responsabilidade do usuário somar o resultado 
 * obtido por cada fluxo.
 * 
 * Entradas esperadas:
 *   arg --> Um ponteiro para inteiro, o número da thread;
 * 
 * Retorna:
 *   sum --> um ponteiro (fazer cast para double *) para o resultado da soma parcial calculada pela thread;
 */
void * integral_discreta_concorrente(void * arg) {
    int * tid = (int *) arg;
    double * sum = (double *) malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    // método do trapézio calculado pelos fluxos de forma alternada
    for (int i = *tid; i < COORD_ARR_SZ - 1; i += NTHREADS) {
        *sum += (Y_COORD[i] + Y_COORD[i + 1]) * (X_COORD[i + 1] - X_COORD[i]);
    }
    *sum /= 2;

    pthread_exit((void *) sum);
}

/* 
 * Esta função utiliza o método dos trapézios para calcular, de forma 
 * concorrente, a integral de uma função contínua à partir de um intervalo 
 * fechado no domínio da função e o número de divisões desejadas neste 
 * intervalo, fica como responsabilidade do usuário somar o resultado 
 * obtido por cada fluxo.
 * 
 * Entradas esperadas:
 *   arg --> Um ponteiro para inteiro, o número da thread;
 * 
 * Retorna:
 *   sum --> um ponteiro (fazer cast para double *) para o resultado da soma parcial calculada pela thread;
 */
void * integral_continua_concorrente(void * arg) {
    int * tid = (int *) arg;
    double distance = (UPPER_EDGE - LOWER_EDGE) / INTERVALS;
    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    if (*tid == 0) {
        *sum = FUNCTION(LOWER_EDGE) + FUNCTION(UPPER_EDGE);
    }
    for (unsigned i = *tid ? *tid : NTHREADS; i < INTERVALS; i += NTHREADS) {
        *sum += 2 * FUNCTION(LOWER_EDGE + distance * i);
    }
    *sum *= distance/2;

    pthread_exit(sum);
}

/* 
 * Esta função utiliza o método dos trapézios para calcular, de forma 
 * concorrente, a integral de uma função contínua à partir de um intervalo 
 * fechado no domínio da função e o número de divisões desejadas neste 
 * intervalo.
 * Esta função também garante que o resultado esteja dentro de um intervalo
 * de precisão determinado atravéz de um limite superior para a segunda 
 * derivada da função e uma tolerância. Fica como responsabilidade do 
 * usuário somar o resultado obtido por cada fluxo.
 * 
 * Entradas esperadas:
 *   arg --> Um ponteiro para inteiro, o número da thread;
 * 
 * Retorna:
 *   sum --> um ponteiro (fazer cast para double *) para o resultado da soma parcial calculada pela thread;
 */
void * integral_continua_com_precisao_concorrente(void * arg) {
    int * tid = (int *) arg;
    // para calcular quantos intervalos devemos usar para chegar a precisão desejada
    // utilizamos a equação de estimativa de erro da interpolação linear
    // E = M(b-a)^3 / 12  , para todo ponto 'c' tal que a <= c <= b e f"(c) <= M
    // dividindo o intervalo [a, b] em n segmentos de tamanha h = (b - a) / n
    // E = nMh^3 / 12 logo, n = sqrt(M(b-a)^3 / 12E)
    unsigned intervals = ceil(sqrt(pow(UPPER_EDGE - LOWER_EDGE, 3) * SECOND_DERIVATIVE_CEIL / (12 * TOLERANCE))); // calculo do número de intervalos
    double distance = (UPPER_EDGE - LOWER_EDGE) / intervals; // calculo do tamanho das subdivisões

    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    if (*tid == 0) {
        *sum = FUNCTION(LOWER_EDGE) + FUNCTION(UPPER_EDGE);
    }

    for (unsigned i = *tid ? *tid : NTHREADS; i < intervals; i += NTHREADS) {
        *sum += 2 * FUNCTION(LOWER_EDGE + distance * i);
    }
    *sum *= distance/2;

    pthread_exit(sum);
}

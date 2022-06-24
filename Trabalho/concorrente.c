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
 *   arg --> Um ponteiro para uma estrutura do tipo 'IContinua_args_t', que contém os campos:
 *     function -> uma função matemática genérica que receba e rotorne um 'double';
 *     intervals -> o número de divisões desejado no intervalo;
 *     lower_edge -> o limite inferior do intervalo, incluso;
 *     upper_edge -> o limite superior do intervalo, incluso;
 *     tid -> o número da thread;
 *     nthreads -> a quantidade de threads paralelas;
 * 
 * Retorna:
 *   sum --> um ponteiro (fazer cast para double *) para o resultado da soma parcial calculada pela thread;
 */
void * integral_continua_concorrente(void * arg) {
    IContinua_args_t * args = (IContinua_args_t *) arg;
    double distance = (args->upper_edge - args->lower_edge) / args->intervals; // calculo do tamanho das subdivisões
    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    // método do trapézio calculado pelos fluxos de forma alternada
    for (double i = args->lower_edge + args->tid * distance; i < args->upper_edge; i += distance * args->nthreads) {
        *sum += (args->function(i) + args->function(i + distance)) * distance;
    }
    *sum /= 2;

    free(args);

    pthread_exit(sum);
}

/* 
 * Esta função utiliza o método dos trapézios para calcular, de forma 
 * concorrente, a integral de uma função contínua à partir de um intervalo 
 * fechado no domínio da função e o número de divisões desejadas neste 
 * intervalo.
 * Esta função também garante que o resultado esteja dentro de um intervalo
 * de precisão determinado atravéz de um limite superior para a segunda 
 * derivada da função e uma tolerância, ambas informações recebidas também 
 * como argumento. Fica como responsabilidade do usuário somar o resultado 
 * obtido por cada fluxo.
 * 
 * Entradas esperadas:
 *   arg --> Um ponteiro para uma estrutura do tipo 'IPrecisao_args_t', que contém os campos:
 *     function -> uma função matemática genérica que receba e rotorne um 'double';
 *     lower_edge -> o limite inferior do intervalo, incluso;
 *     upper_edge -> o limite superior do intervalo, incluso;
 *     precision -> o número de divisões desejado no intervalo;
 *     second_derivative_ceil -> o teto para a segunda derivada no intervalo;
 *     tid -> o número da thread;
 *     nthreads -> a quantidade de threads paralelas;
 * 
 * Retorna:
 *   sum --> um ponteiro (fazer cast para double *) para o resultado da soma parcial calculada pela thread;
 */
void * integral_continua_com_precisao_concorrente(void * arg) {
    IPrecisao_args_t * args = (IPrecisao_args_t *) arg;
    // para calcular quantos intervalos devemos usar para chegar a precisão desejada
    // utilizamos a equação de estimativa de erro da interpolação linear
    // E = M(b-a)^3 / 12  , para todo ponto 'c' tal que a <= c <= b e f"(c) <= M
    // dividindo o intervalo [a, b] em n segmentos de tamanha h = (b - a) / n
    // E = nMh^3 / 12 logo, n = sqrt(M(b-a)^3 / 12E)
    unsigned intervals = ceil(sqrt(pow(args->upper_edge - args->lower_edge, 3) * args->second_derivative_ceil / (12 * args->precision))); // calculo do número de intervalos
    double distance = (args->upper_edge - args->lower_edge) / intervals; // calculo do tamanho das subdivisões

    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    // método do trapézio calculado pelos fluxos de forma alternada
    for (double i = args->lower_edge + args->tid * distance; i < args->upper_edge; i += distance * args->nthreads) {
        *sum += (args->function(i) + args->function(i + distance)) * distance;
    }
    *sum /= 2;

    free(args);

    pthread_exit(sum);
}

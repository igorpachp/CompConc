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
    // similar ao método sequencial
    for (unsigned i = *tid; i < COORD_ARR_SZ - 1; i += NTHREADS) {
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
    // calculo da altura do trapézio
    double distance = (UPPER_EDGE - LOWER_EDGE) / INTERVALS;
    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    // calculo de f(a) e f(b), onde 'a' e 'b' são os limites do intervalo 
    // será sempre executado sempre pelo fluxo com identificador 0
    // pois diferente dos outro pontos, f(a) e f(b) devem ser considerados
    // uma única vez na soma da área dos trapézios
    if (*tid == 0) {
        *sum = FUNCTION(LOWER_EDGE) + FUNCTION(UPPER_EDGE);
    }
    // no calculo da área do trapézio para uma função discreta, 
    // veremos que ao calcular o trapézio de número 'n' teremos 
    // Sn = ( f(n) + f(n+1) ) * h / 2
    // e para o calculo do trapézio 'n+1'
    // Sn+1 = ( f(n+1) + f(n+2) ) * h / 2
    // perceba que uma das bases é considerada duas vezes para 
    // trapézios consecutivos, utilizaremos este fato para 
    // economizar algumas iterações no laço, já que o calculo de f(n)
    // pode ser custoso, dependendo da função recebida
    for (unsigned i = *tid ? *tid : NTHREADS; i < INTERVALS; i += NTHREADS) {
        *sum += 2 * FUNCTION(LOWER_EDGE + distance * i);
    }
    // ainda economizando operações, visto que a distância entre os pontos 
    // é fixa na implementação contínua, podemos executar tanto a divisão
    // quanto a multiplicação após a execução do laço
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
    // para calcular quantos intervalos devemos usar para chegar a precisão desejada
    // utilizamos a equação de estimativa de erro da interpolação linear
    // E = M(b-a)^3 / 12  , para todo ponto 'c' tal que a <= c <= b e f"(c) <= M
    // dividindo o intervalo [a, b] em n segmentos de tamanha h = (b - a) / n
    // E = nMh^3 / 12 logo, n = sqrt(M(b-a)^3 / 12E)
    INTERVALS = ceil(sqrt(pow(UPPER_EDGE - LOWER_EDGE, 3) * SECOND_DERIVATIVE_CEIL / (12 * TOLERANCE))); // calculo do número de intervalos

    pthread_exit(integral_continua_concorrente(arg));
}

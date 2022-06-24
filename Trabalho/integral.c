#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "sequencial.h"
#include "concorrente.h"
#include "../lib/timer.h"

#define DEFAULT_NTHREADS    4
#define DEFAULT_RANGE       10
#define DEFAULT_PRECISION   pow(10, -6)
#define DEFAULT_DERIVATIVE  4.62

double * range(double lower_edge, double upper_edge, unsigned n, double (*function)(double)) {
    double * arr = (double *) malloc(sizeof(double) * (n + 1));
    if (!arr) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }
    double step = (upper_edge - lower_edge) / (n);
    
    *arr = !function ? lower_edge : function(lower_edge);
    *(arr + n) = !function ? upper_edge : function(upper_edge);
    for (int i = 1; i < n; i++) {
        *(arr + i) = !function ? *(arr + i - 1) + step : function(lower_edge + i * step);
    }

    return arr;
}

double reference_function(double x) {
    return 0.5 * sin(3 * x) + 3 * cos(x / 5);
}

int main(int argc, char * argv[]) {
    // VARIAVEIS PARA MEDIÇÃO DE TEMPO
    double start_s, end_s, elapsed_s;
    double start_c, end_c, elapsed_c;

    // VARIAVEIS PARA GUARDAR RESULTADO
    double integral;
    double * resultado;

    // LIMITES DE INTEGRAÇÃO
    unsigned intervals;
    double upper, lower;

    // RECEBENDO ARGUMENTOS DA LINHA DE COMANDO
    switch (argc)
    {
        case 1:
            NTHREADS = DEFAULT_NTHREADS;
            INTERVALS = intervals = DEFAULT_RANGE;
            COORD_ARR_SZ = INTERVALS + 1;
            LOWER_EDGE = lower = -1.0;
            UPPER_EDGE = upper = 1.0;
            X_COORD = range(LOWER_EDGE, UPPER_EDGE, INTERVALS, NULL);
            Y_COORD = range(X_COORD[0], X_COORD[INTERVALS], INTERVALS, reference_function);
            break;
        case 5:
            INTERVALS = intervals = atoi(argv[3]);
            COORD_ARR_SZ = INTERVALS + 1;
            NTHREADS = atoi(argv[4]);
            LOWER_EDGE = lower = strtod(argv[1], NULL);
            UPPER_EDGE = upper = strtod(argv[2], NULL);
            X_COORD = range(LOWER_EDGE, UPPER_EDGE, INTERVALS, NULL);
            Y_COORD = range(X_COORD[0], X_COORD[INTERVALS], INTERVALS, reference_function);
            break;
        default:
            fprintf(stderr, "--ERRO: Numero de parametros incorreto!\n");
            fprintf(stderr, "Forma de uso: %s <limite-inferior> <limite superior> <numero-de-intervalos*> <numero-de-threads*>\n", argv[0]);
            fprintf(stderr, "Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // VARIAVEIS CONCORRENTES
    pthread_t tid_sys[NTHREADS];
    int inner_tid[NTHREADS];
    IPrecisao_args_t * args_precisao;

    // TESTE SEQUENCIAL: FUNÇÃO DISCRETA ===============================================
    GET_TIME(start_s);
    integral = integral_discreta_sequencial(X_COORD, Y_COORD, COORD_ARR_SZ);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("resultado sequencial discreta:  %lf\n", integral);
    // FIM TESTE =======================================================================

    integral = 0;
    // TESTE CONCORRENTE: FUNÇÃO DISCRETA ==============================================
    GET_TIME(start_c);
    // criando as threads da função discreta
    for (int i = 0; i < NTHREADS; i++) {
        // preenchendo argumentos da função discreta
        inner_tid[i] = i;

        // criando threads da função discreta
        if(pthread_create(&tid_sys[i], NULL, integral_discreta_concorrente, (void *) &inner_tid[i])) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-2);
        }
    }

    // recebendo resultados
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid_sys + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
        free(resultado);
    }

    GET_TIME(end_c);
    elapsed_c = end_c - start_c;
    printf("resultado concorrente discreta: %lf\n", integral);
    // FIM TESTE =======================================================================
    printf("FUNCAO DISCRETA -------------------\n");
    printf("\ttempo sequencial:  %lf\n", elapsed_s);
    printf("\ttempo concorrente: %lf\n", elapsed_c);
    printf("\taceleracao: %lf\n\n", elapsed_s / elapsed_c);

    // TESTE SEQUENCIAL: FUNÇÃO CONTINUA ===============================================
    GET_TIME(start_s);
    integral = integral_continua_sequencial(reference_function, INTERVALS, LOWER_EDGE, UPPER_EDGE);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("sequencial continua: %lf\n", integral);
    // FIM TESTE =======================================================================

    integral = 0;
    // TESTE CONCORRENTE: FUNÇÃO CONTINUA ==============================================
    GET_TIME(start_c);
    FUNCTION = reference_function;
    // criando as threads da função continua
    for (int i = 0; i < NTHREADS; i++) {
        // preenchendo argumentos da função continua
        inner_tid[i] = i;

        // criando threads da função continua
        if(pthread_create(&tid_sys[i], NULL, integral_continua_concorrente, (void *) &inner_tid[i])) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-2);
        }
    }

    // recebendo resultados
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid_sys + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
        free(resultado);
    }

    GET_TIME(end_c);
    elapsed_c = end_c - start_c;
    printf("concorrente continua: %lf\n", integral);
    // FIM TESTE =======================================================================
    printf("FUNCAO CONTINUA -------------------\n");
    printf("\ttempo sequencial:  %lf\n", elapsed_s);
    printf("\ttempo concorrente: %lf\n", elapsed_c);
    printf("\taceleracao: %lf\n\n", elapsed_s / elapsed_c);

    // TESTE SEQUENCIAL: FUNÇÃO COM PRECISAO ===========================================
    GET_TIME(start_s);
    integral = integral_continua_com_precisao_sequencial(reference_function, lower, upper, DEFAULT_PRECISION, DEFAULT_DERIVATIVE);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("sequencial precisao: %lf\n", integral);
    // FIM TESTE =======================================================================

    integral = 0;
    // TESTE CONCORRENTE: FUNÇÃO COM PRECISAO ==========================================
    GET_TIME(start_c);
    // criando as threads da função discreta
    for (int i = 0; i < NTHREADS; i++) {
        // alocando argumentos da função com precisao
        args_precisao = (IPrecisao_args_t *) malloc(sizeof(IPrecisao_args_t));
        if (!args_precisao) {
            fprintf(stderr, "--ERRO: malloc()\n");
            exit(-1);
        }

        // preenchendo argumentos da função com precisao
        args_precisao->function = reference_function;
        args_precisao->lower_edge = lower;
        args_precisao->upper_edge = upper;
        args_precisao->precision = DEFAULT_PRECISION;
        args_precisao->second_derivative_ceil = DEFAULT_DERIVATIVE;
        args_precisao->tid = i;
        args_precisao->nthreads = NTHREADS;

        // criando threads da função com precisao
        if(pthread_create(&tid_sys[i], NULL, integral_continua_com_precisao_concorrente, (void *) args_precisao)) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-2);
        }
    }

    // recebendo resultados
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid_sys + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
        free(resultado);
    }

    GET_TIME(end_c);
    elapsed_c = end_c - start_c;
    printf("resultado concorrente precisao: %lf\n", integral);
    // FIM TESTE =======================================================================
    printf("FUNCAO COM PRECISAO -------------------\n");
    printf("\ttempo sequencial:  %lf\n", elapsed_s);
    printf("\ttempo concorrente: %lf\n", elapsed_c);
    printf("\taceleracao: %lf\n\n", elapsed_s / elapsed_c);

    free(X_COORD);
    free(Y_COORD);

    return 0;
}

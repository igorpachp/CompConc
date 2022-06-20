#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "sequencial.h"
#include "concorrente.h"
#include "../lib/timer.h"

#define DEFAULT_NTHREADS    4
#define DEFAULT_RANGE       10
#define DEFAULT_PRECISION   pow(10, -15)
#define DEFAULT_DERIVATIVE  4.62

unsigned NTHREADS;

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

    // ARRAYS DE REFERENCIA
    double * x;
    double * y;

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
            intervals = DEFAULT_RANGE;
            lower = -1.0;
            upper = 1.0;
            x = range(lower, upper, intervals, NULL);
            y = range(x[0], x[intervals], intervals, reference_function);
            break;
        case 5:
            intervals = atoi(argv[3]);
            NTHREADS = atoi(argv[4]);
            lower = strtod(argv[1], NULL);
            upper = strtod(argv[2], NULL);
            x = range(lower, upper, intervals, NULL);
            y = range(x[0], x[intervals], intervals, reference_function);
            break;
        default:
            fprintf(stderr, "--ERRO: Numero de parametros incorreto!\n");
            fprintf(stderr, "Forma de uso: %s <limite-inferior> <limite superior> <numero-de-intervalos*> <numero-de-threads*>\n", argv[0]);
            fprintf(stderr, "Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // VARIAVEIS CONCORRENTES
    pthread_t tid[NTHREADS];
    IDiscreta_args_t * args_discreta;
    IContinua_args_t * args_continua;
    IPrecisao_args_t * args_precisao;

    // TESTE SEQUENCIAL: FUNÇÃO DISCRETA ===============================================
    GET_TIME(start_s);
    integral = integral_discreta_sequencial(x, y, intervals+1);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("resultado sequencial discreta: %lf\n", integral);
    // FIM TESTE =======================================================================

    integral = 0;
    // TESTE CONCORRENTE: FUNÇÃO DISCRETA ==============================================
    GET_TIME(start_c);
    // criando as threads da função discreta
    for (int i = 0; i < NTHREADS; i++) {
        // alocando argumentos da função discreta
        args_discreta = (IDiscreta_args_t *) malloc(sizeof(IDiscreta_args_t));
        if (!args_discreta) {
            fprintf(stderr, "--ERRO: malloc()\n");
            exit(-1);
        }

        // preenchendo argumentos da função discreta
        args_discreta->x = x;
        args_discreta->y = y;
        args_discreta->size = intervals + 1;
        args_discreta->tid = i;
        args_discreta->nthreads = NTHREADS;

        // criando threads da função discreta
        if(pthread_create(&tid[i], NULL, integral_discreta_concorrente, (void *) args_discreta)) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-2);
        }
    }

    // recebendo resultados
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
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
    integral = integral_continua_sequencial(reference_function, intervals, lower, upper);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("sequencial continua: %lf\n", integral);
    // FIM TESTE =======================================================================

    integral = 0;
    // TESTE CONCORRENTE: FUNÇÃO CONTINUA ==============================================
    GET_TIME(start_c);
    // criando as threads da função continua
    for (int i = 0; i < NTHREADS; i++) {
        // alocando argumentos da função continua
        args_continua = (IContinua_args_t *) malloc(sizeof(IContinua_args_t));
        if (!args_continua) {
            fprintf(stderr, "--ERRO: malloc()\n");
            exit(-1);
        }

        // preenchendo argumentos da função continua
        args_continua->function = reference_function;
        args_continua->intervals = intervals;
        args_continua->lower_edge = lower;
        args_continua->upper_edge = upper;
        args_continua->tid = i;
        args_continua->nthreads = NTHREADS;

        // criando threads da função continua
        if(pthread_create(&tid[i], NULL, integral_continua_concorrente, (void *) args_continua)) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-2);
        }
    }

    // recebendo resultados
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
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
        if(pthread_create(&tid[i], NULL, integral_continua_com_precisao_concorrente, (void *) args_precisao)) {
            fprintf(stderr, "--ERRO: pthread_create()\n");
            exit(-2);
        }
    }

    // recebendo resultados
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
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

    free(x);
    free(y);

    return 0;
}

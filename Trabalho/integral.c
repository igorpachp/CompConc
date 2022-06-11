#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "sequencial.h"
#include "concorrente.h"

#define DEFAULT_NTHREADS    4
#define DEFAULT_RANGE       10
#define DEFAULT_PRECISION   pow(10, -6)

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
    double * x;
    double * y;
    unsigned intervals;
    double upper, lower;

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
            puts("--ERRO: Numero de parametros incorreto!\n");
            puts("Forma de uso: ./<app> <limite-inferior> <limite superior> <numero-de-intervalos*> <numero-de-threads*>\n");
            puts("Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    double integral = 0;
    double * resultado;
    pthread_t tid[NTHREADS];
    IDiscreta_args_t ** args_discreta;
    IContinua_args_t ** args_continua;
    IPrecisao_args_t ** args_precisao;

    printf("sequencial discreta: %lf\n", integral_discreta_sequencial(x, y, intervals+1));
    printf("sequencial continua: %lf\n", integral_continua_sequencial(reference_function, intervals, lower, upper));
    printf("sequencial precisao: %lf\n", integral_continua_com_precisao_sequencial(reference_function, lower, upper, DEFAULT_PRECISION, 4.62));

    args_discreta = (IDiscreta_args_t **) malloc(sizeof(IDiscreta_args_t *) * NTHREADS);
    if (!args_discreta) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }
    FILL_DISCRETA_ARGS(args_discreta, intervals + 1, x, y, NTHREADS);
    CREATE_THREADS(tid, NULL, integral_discreta_concorrente, args_discreta, NTHREADS);

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
    }
    printf("concorrente discreta: %lf\n", integral);
    free(resultado);

    integral = 0;
    args_continua = (IContinua_args_t **) malloc(sizeof(IContinua_args_t *) * NTHREADS);
    if (!args_continua) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }
    FILL_CONTINUA_ARGS(args_continua, reference_function, intervals, lower, upper, NTHREADS);
    CREATE_THREADS(tid, NULL, integral_continua_concorrente, args_continua, NTHREADS);
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
    }
    printf("concorrente continua: %lf\n", integral);

    integral = 0;
    args_precisao = (IPrecisao_args_t **) malloc(sizeof(IPrecisao_args_t *) * NTHREADS);
    if (!args_precisao) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }
    FILL_PRECISAO_ARGS(args_precisao, reference_function, lower, upper, DEFAULT_PRECISION, 4.62, NTHREADS);
    CREATE_THREADS(tid, NULL, integral_continua_com_precisao_concorrente, args_precisao, NTHREADS);
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
    }
    printf("concorrente precisao: %lf\n", integral);

    free(resultado);
    free(args_discreta);
    free(args_continua);
    free(args_precisao);

    return 0;
}

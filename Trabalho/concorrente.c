#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "concorrente.h"

double calcula_trapezio (double a, double b, double fa, double fb) {
    return (fa + fb) * (b - a) / 2;
}

void * integral_discreta_concorrente(void * arg) {
    IDiscreta_args_t * args = (IDiscreta_args_t *) arg;
    double * sum = (double *) malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    int first = args->tid * ((args->size - 1) / NTHREADS);
    int last = (args->tid != (NTHREADS - 1)) ? (args->tid + 1) * ((args->size - 1) / NTHREADS) : args->size - 1;

    for (int i = first; i < last; i++) {
        *sum += calcula_trapezio(args->x[i], args->x[i + 1], args->y[i], args->y[i + 1]);
    }

    free(args);

    pthread_exit((void *) sum);
}

void * integral_continua_concorrente(void * arg) {
    IContinua_args_t * args = (IContinua_args_t *) arg;
    double distance = (args->upper_edge - args->lower_edge) / args->intervals;
    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    double first = args->lower_edge + (distance * (args->intervals / NTHREADS)) * args->tid;
    double last = (args->tid != (NTHREADS - 1)) ? args->lower_edge + (distance * (args->intervals / NTHREADS)) * (args->tid + 1) : args->upper_edge;

    double a = first;
    for (int i = 0; i < round((last - first) / distance); i++) {
        double b = first + distance * (i + 1);
        *sum += calcula_trapezio(a, b, args->function(a), args->function(b));
        a = b;
    }

    free(args);

    pthread_exit(sum);
}

void * integral_continua_com_precisao_concorrente(void * arg) {
    IPrecisao_args_t * args = (IPrecisao_args_t *) arg;
    unsigned intervals = ceil(sqrt(pow(args->upper_edge - args->lower_edge, 3) * args->second_derivative_ceil / (12 * args->precision)));
    double distance = (args->upper_edge - args->lower_edge) / intervals;

    double * sum = malloc(sizeof(double));
    if (!sum) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }

    double first = args->lower_edge + (distance * (intervals / NTHREADS)) * args->tid;
    double last = (args->tid != (NTHREADS - 1)) ? args->lower_edge + (distance * (intervals / NTHREADS)) * (args->tid + 1) : args->upper_edge;

    double a = first;
    for (int i = 0; i < round((last - first) / distance); i++) {
        double b = first + distance * (i + 1);
        *sum += calcula_trapezio(a, b, args->function(a), args->function(b));
        a = b;
    }
    free(args);

    pthread_exit(sum);
}

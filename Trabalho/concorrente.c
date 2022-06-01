#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "concorrente.h"

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
        *sum += (args->y[i] + args->y[i + 1]) * (args->x[i + 1] - args->x[i]);
    }

    free(args);
    *sum /= 2;

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

    *sum = args->function(first) + args->function(last);

    for (int i = 1; i < round((last - first) / distance); i++) {
        *sum += 2 * args->function(first + distance * i);
    }

    free(args);
    *sum *= distance / 2;

    pthread_exit(sum);
}

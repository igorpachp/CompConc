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

    int first = args->tid * args->size / NTHREADS;
    int last = (args->tid != (NTHREADS - 1)) ? (args->tid + 1) * (args->size - 1) / NTHREADS : args->size;

    printf("size: %d\t\tprimeiro: %d\nultimo: %d\n",args->size, first, last);
    for (int i = first; i < last; i++) {
        *sum += (args->y[i] + args->y[i + 1]) * (args->x[i + 1] - args->x[i]);
        printf("passo: %d\t\tsum: %lf\n", i, *sum);
    }

    free(args);

    *sum /= 2;
    puts("aqui");

    pthread_exit((void *) sum);
}

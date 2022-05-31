#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "sequencial.h"
#include "concorrente.h"
#include "macros.h"

unsigned NTHREADS = 4;

double reference_function(double x) {
    return 0.5 * sin(3 * x) + 3 * cos(x / 5);
}

int main() {
    double x[] = {-1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
    double y[sizeof(x) / sizeof(double)];
    pthread_t tid[NTHREADS];

    for (int i = 0; i < sizeof(x) / sizeof(double); i++) {
        y[i] = reference_function(x[i]);
    }

    printf("%lf\n", integral_discreta_sequencial(x, y, sizeof(x) / sizeof(double)));
    printf("%lf\n", integral_continua_sequencial(reference_function, 10, -1, 1));
    printf("%lf\n", integral_continua_com_precisao_sequencial(reference_function, -1, 1, 0.01, 4.62));

    IDiscreta_args_t ** args = (IDiscreta_args_t **) malloc(sizeof(IDiscreta_args_t *) * NTHREADS);
    if (!args) {
        fprintf(stderr, "--ERRO: malloc()\n");
        exit(-1);
    }
    FILL_DISCRETA_ARGS(args, sizeof(x) / sizeof(double), &x, &y, NTHREADS);
    CREATE_THREADS(tid, NULL, integral_discreta_concorrente, args, NTHREADS);
    
    double integral = 0;
    double * resultado;
    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_join(*(tid + i), (void **) &resultado)) {
            fprintf(stderr, "--ERRO: pthread_join()\n");
            exit(-4);
        }
        integral += *resultado;
    }
    printf("%lf\n", integral);

    free(resultado);
    free(args);

    return 0;
}
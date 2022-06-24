#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include "sequencial.h"
#include "concorrente.h"
#include "../lib/timer.h"

#define DEFAULT_NTHREADS    4
#define DEFAULT_RANGE       10
#define DEFAULT_PRECISION   -6
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

void * fteste(void * arg) {
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

int main(int argc, char * argv[]) {
    // VARIAVEIS PARA MEDIÇÃO DE TEMPO
    double start_s, end_s, elapsed_s;
    double start_c, end_c, elapsed_c;

    // VARIAVEIS PARA GUARDAR RESULTADO
    double integral;
    double * resultado;

    // RECEBENDO ARGUMENTOS DA LINHA DE COMANDO
    switch (argc)
    {
        case 1:
            NTHREADS = DEFAULT_NTHREADS;
            INTERVALS = DEFAULT_RANGE;
            COORD_ARR_SZ = INTERVALS + 1;
            LOWER_EDGE = -1.0;
            UPPER_EDGE = 1.0;
            X_COORD = range(LOWER_EDGE, UPPER_EDGE, INTERVALS, NULL);
            Y_COORD = range(X_COORD[0], X_COORD[INTERVALS], INTERVALS, reference_function);
            TOLERANCE = pow(10, DEFAULT_PRECISION);
            SECOND_DERIVATIVE_CEIL = DEFAULT_DERIVATIVE;
            break;
        case 5:
            INTERVALS = atoi(argv[3]);
            COORD_ARR_SZ = INTERVALS + 1;
            NTHREADS = atoi(argv[4]);
            LOWER_EDGE = strtod(argv[1], NULL);
            UPPER_EDGE = strtod(argv[2], NULL);
            X_COORD = range(LOWER_EDGE, UPPER_EDGE, INTERVALS, NULL);
            Y_COORD = range(X_COORD[0], X_COORD[INTERVALS], INTERVALS, reference_function);
            TOLERANCE = pow(10, DEFAULT_PRECISION);
            SECOND_DERIVATIVE_CEIL = DEFAULT_DERIVATIVE;
            break;
        case 6:
            INTERVALS = atoi(argv[3]);
            COORD_ARR_SZ = INTERVALS + 1;
            NTHREADS = atoi(argv[4]);
            LOWER_EDGE = strtod(argv[1], NULL);
            UPPER_EDGE = strtod(argv[2], NULL);
            X_COORD = range(LOWER_EDGE, UPPER_EDGE, INTERVALS, NULL);
            Y_COORD = range(X_COORD[0], X_COORD[INTERVALS], INTERVALS, reference_function);
            TOLERANCE = pow(10, atoi(argv[5]));
            SECOND_DERIVATIVE_CEIL = DEFAULT_DERIVATIVE;
            break;
        case 7:
            INTERVALS = atoi(argv[3]);
            COORD_ARR_SZ = INTERVALS + 1;
            NTHREADS = atoi(argv[4]);
            LOWER_EDGE = strtod(argv[1], NULL);
            UPPER_EDGE = strtod(argv[2], NULL);
            X_COORD = range(LOWER_EDGE, UPPER_EDGE, INTERVALS, NULL);
            Y_COORD = range(X_COORD[0], X_COORD[INTERVALS], INTERVALS, reference_function);
            TOLERANCE = pow(10, atoi(argv[5]));
            SECOND_DERIVATIVE_CEIL = atoi(argv[6]);
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
    integral = integral_continua_com_precisao_sequencial(reference_function, LOWER_EDGE, UPPER_EDGE, TOLERANCE, SECOND_DERIVATIVE_CEIL);
    GET_TIME(end_s);
    elapsed_s = end_s - start_s;
    printf("sequencial precisao: %lf\n", integral);
    // FIM TESTE =======================================================================

    integral = 0;
    // TESTE CONCORRENTE: FUNÇÃO COM PRECISAO ==========================================
    GET_TIME(start_c);
    FUNCTION = reference_function;
    // criando as threads da função discreta
    for (int i = 0; i < NTHREADS; i++) {
        // preenchendo argumentos da função com precisao
        inner_tid[i] = i;

        // criando threads da função com precisao
        if(pthread_create(&tid_sys[i], NULL, fteste, (void *) &inner_tid[i])) {
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

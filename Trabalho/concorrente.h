#ifndef __CONCORRENTE_H__
#define __CONCORRENTE_H__

// VARIAVEIS GERAIS
unsigned NTHREADS;

// VARIAVEIS DISCRETAS
double * X_COORD;
double * Y_COORD;
unsigned COORD_ARR_SZ;

typedef struct {
    double (*function)(double);
    unsigned intervals;
    double lower_edge;
    double upper_edge;
    unsigned tid;
    unsigned nthreads;
} IContinua_args_t;

typedef struct {
    double (*function)(double);
    double lower_edge;
    double upper_edge;
    double precision;
    double second_derivative_ceil;
    unsigned tid;
    unsigned nthreads;
} IPrecisao_args_t;

void * integral_discreta_concorrente(void *);
void * integral_continua_concorrente(void *);
void * integral_continua_com_precisao_concorrente(void *);

#endif

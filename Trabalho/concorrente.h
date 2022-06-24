#ifndef __CONCORRENTE_H__
#define __CONCORRENTE_H__

// VARIAVEIS GERAIS
unsigned NTHREADS;

// VARIAVEIS DISCRETAS
double * X_COORD;
double * Y_COORD;
unsigned COORD_ARR_SZ;

// VARIAVEIS CONTINUAS
double (*FUNCTION)(double);
unsigned INTERVALS;
double LOWER_EDGE;
double UPPER_EDGE;

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

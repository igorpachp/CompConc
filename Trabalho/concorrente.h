#ifndef __CONCORRENTE_H__
#define __CONCORRENTE_H__

typedef struct {
    double * x;
    double * y;
    unsigned size;
    unsigned tid;
    unsigned nthreads;
} IDiscreta_args_t;

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

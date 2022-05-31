#ifndef __CONCORRENTE_H__
#define __CONCORRENTE_H__

typedef struct {
    double * x;
    double * y;
    unsigned size;
    unsigned tid;
    unsigned nthreads;
} IDiscreta_args_t;

extern unsigned NTHREADS;

void * integral_discreta_concorrente(void *);

#endif

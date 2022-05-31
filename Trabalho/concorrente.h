#ifndef __CONCORRENTE_H__
#define __CONCORRENTE_H__

#define FILL_DISCRETA_ARGS(__ARGSPTR, __SIZE, __XPTR, __YPTR, __NTHREADS) {         \
    for (int i = 0; i < __NTHREADS; i++) {                                          \
        *(__ARGSPTR + i) = (IDiscreta_args_t *) malloc(sizeof(IDiscreta_args_t));   \
        if (!__ARGSPTR) {                                                           \
            fprintf(stderr, "--ERRO: malloc()\n");                                  \
            exit(-1);                                                               \
        }                                                                           \
        (*(__ARGSPTR + i))->size = __SIZE;                                          \
        (*(__ARGSPTR + i))->tid = i;                                                \
        (*(__ARGSPTR + i))->x = __XPTR;                                             \
        (*(__ARGSPTR + i))->y = __YPTR;                                             \
    }                                                                               \
}

#define CREATE_THREADS(__TIDPTR, __ATTR, __ROUTINE, __ARGSPTR, __NTHREADS) {                \
    for(int i = 0; i < __NTHREADS; i++) {                                                   \
        if(pthread_create(__TIDPTR + i, __ATTR, __ROUTINE, (void *) *(__ARGSPTR + i))) {    \
            fprintf(stderr, "--ERRO: pthread_create()\n");                                  \
            exit(-2);                                                                       \
        }                                                                                   \
    }                                                                                       \
}

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

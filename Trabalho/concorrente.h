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

#define FILL_CONTINUA_ARGS(__ARGSPTR, __FPTR, __INTERVALS, __LEDGE, __UEDGE, __NTHREADS) {      \
    for (int i = 0; i < __NTHREADS; i++) {                                                      \
        *(__ARGSPTR + i) = (IContinua_args_t *) malloc(sizeof(IContinua_args_t));               \
        if (!__ARGSPTR) {                                                                       \
            fprintf(stderr, "--ERRO: malloc()\n");                                              \
            exit(-1);                                                                           \
        }                                                                                       \
        (*(__ARGSPTR + i))->function = __FPTR;                                                  \
        (*(__ARGSPTR + i))->intervals = __INTERVALS;                                            \
        (*(__ARGSPTR + i))->lower_edge = __LEDGE;                                               \
        (*(__ARGSPTR + i))->upper_edge = __UEDGE;                                               \
        (*(__ARGSPTR + i))->tid = i;                                                            \
    }                                                                                           \
}

#define FILL_PRECISAO_ARGS(__ARGSPTR, __FPTR, __LEDGE, __UEDGE, __PRECISION, __DERIV, __NTHREADS) {     \
    for (int i = 0; i < __NTHREADS; i++) {                                                              \
        *(__ARGSPTR + i) = (IPrecisao_args_t *) malloc(sizeof(IPrecisao_args_t));                       \
        if (!__ARGSPTR) {                                                                               \
            fprintf(stderr, "--ERRO: malloc()\n");                                                      \
            exit(-1);                                                                                   \
        }                                                                                               \
        (*(__ARGSPTR + i))->function = __FPTR;                                                          \
        (*(__ARGSPTR + i))->lower_edge = __LEDGE;                                                       \
        (*(__ARGSPTR + i))->upper_edge = __UEDGE;                                                       \
        (*(__ARGSPTR + i))->precision = __PRECISION;                                                    \
        (*(__ARGSPTR + i))->second_derivative_ceil = __DERIV;                                           \
        (*(__ARGSPTR + i))->tid = i;                                                                    \
    }                                                                                                   \
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
} IDiscreta_args_t;

typedef struct {
    double (*function)(double);
    unsigned intervals;
    double lower_edge;
    double upper_edge;
    unsigned tid;
} IContinua_args_t;

typedef struct {
    double (*function)(double);
    double lower_edge;
    double upper_edge;
    double precision;
    double second_derivative_ceil;
    unsigned tid;
} IPrecisao_args_t;

extern unsigned NTHREADS;

void * integral_discreta_concorrente(void *);
void * integral_continua_concorrente(void *);
void * integral_continua_com_precisao_concorrente(void *);

#endif

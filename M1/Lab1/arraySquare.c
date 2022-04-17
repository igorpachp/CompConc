/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: "Hello World" usando threads em C */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define ARR_LEN  10
#define NTHREADS 1

typedef struct {
	int * arr;
	unsigned len;
} square_args_t;

void * array_square(void * arg) {
	square_args_t * args = (square_args_t *) arg;
		
	int * tail = args->arr + args->len - 1;
	while (args->arr <= tail) {
		*args->arr *= *args->arr;
		args->arr++;
	}

	free(arg);
	pthread_exit(NULL);
}

int main(void) {
	int arr[ARR_LEN];
	pthread_t tid;
	square_args_t * arg;

	srand(time(NULL));
	for (int i = 0; i < ARR_LEN; i++) {
		arr[i] = rand() % 10;
	}

	printf("vetor antes:\t");
	for (int i = 0; i < ARR_LEN; i++) {
		printf("%d ", arr[i]);
	}
	puts("");

	arg = malloc(sizeof(square_args_t));
	if (!arg) {
		printf("--ERRO: malloc()\n");
		exit(-1);
	}
	arg->arr = &arr[0];
	arg->len = ARR_LEN;
	
	if (pthread_create(&tid, NULL, array_square, (void *) arg)) {
		printf("--ERRO: pthread_create()\n");
		exit(-1);
	}
	
	pthread_join(tid, NULL);

	printf("vetor depois:\t");
	for (int i = 0; i < ARR_LEN; i++) {
		printf("%d ", arr[i]);
	}
	puts("");

	return 0;
}

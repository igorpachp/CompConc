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
#define NTHREADS 2

/* 
 * Estrutura para agrupar os argumentos esperados
 * pela função 'array_square'
 */
typedef struct {
	int * arr;
	unsigned len;
} array_square_args_t;

/* 
 * Esta função calcula o quadrado dos números de um
 * vetor de inteiros, salvando o resultado no próprio
 * vetor de forma que o resultado de um elemento fique
 * no lugar previamente ocupado por ele
 * Entradas esperadas:
 *   arr --> um ponteiro para o primeiro elemento
 *   len --> o número de elementos no vetor
 */
void * array_square(void * arg) {
	array_square_args_t * args = (array_square_args_t *) arg;

	int * tail = args->arr + args->len - 1; // guardando posição do último elemento
	while (args->arr <= tail) {
		*args->arr *= *args->arr;
		args->arr++;
	}

	free(arg); // liberando a memoria alocada para os argumentos
	pthread_exit(NULL);
}

int main(void) {
	int arr[ARR_LEN];
	pthread_t tid[NTHREADS];  // identificadores das threads no sistema
	array_square_args_t * arg;  // ponteiro para os futuros argumentos

	// populando o vetor aleatóriamente
	srand(time(NULL));
	for (int i = 0; i < ARR_LEN; i++) {
		arr[i] = rand() % 10;
	}

	// exibindo vetor antes das operações
	printf("vetor antes:\t");
	for (int i = 0; i < ARR_LEN; i++) {
		printf("%d ", arr[i]);
	}
	puts("");

	// gerando estrutura com os argumentos
	arg = malloc(sizeof(array_square_args_t));
	if (!arg) {
		printf("--ERRO: malloc()\n");
		exit(-1);
	}
	arg->arr = &arr[0];
	arg->len = ARR_LEN;
	
	// criando a thread de execução
	if (pthread_create(&tid, NULL, array_square, (void *) arg)) {
		printf("--ERRO: pthread_create()\n");
		exit(-1);
	}
	
	// esperando o término da thread
	pthread_join(tid, NULL);

	// exibindo vetor depois das operações
	printf("vetor depois:\t");
	for (int i = 0; i < ARR_LEN; i++) {
		printf("%d ", arr[i]);
	}
	puts("");

	return 0;
}

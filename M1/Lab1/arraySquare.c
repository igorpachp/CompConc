/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Quadrado dos elementos de um array */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define ARR_LEN  10000
#define NTHREADS 2

/* 
 * Estrutura para agrupar os argumentos esperados
 * pela função 'array_square'
 */
typedef struct {
	int * head;
	int * tail;
} array_square_args_t;

/* 
 * Esta função calcula o quadrado dos números de um
 * vetor de inteiros, salvando o resultado no próprio
 * vetor de forma que o resultado de um elemento fique
 * no lugar previamente ocupado por ele
 * Entradas esperadas:
 *   head --> um ponteiro para o primeiro elemento
 *   tail --> um ponteiro para o último elemento
 */
void * array_square(void * arg) {
	array_square_args_t * args = (array_square_args_t *) arg;

	while (args->head <= args->tail) {
		*args->head *= *args->head;
		args->head++;
	}

	free(arg); // liberando a memoria alocada para os argumentos
	pthread_exit(NULL);
}

int main(void) {
	int arr[ARR_LEN];
	int reference[ARR_LEN];
	pthread_t tid[NTHREADS];  // identificadores das threads no sistema
	array_square_args_t * arg;  // ponteiro para os futuros argumentos
	int * arr_heads[NTHREADS];  // primeiro elemento do pedaço recebido por uma thread
	int * arr_tails[NTHREADS];  // último elemento (incluso) do pedaço recebido por uma thread
	short err_flag = 0;  // guarda a quantidade de elementos incorretos

	puts("populando o vetor aleatoriamente...");
	srand(time(NULL));
	for (int i = 0; i < ARR_LEN; i++) {
		arr[i] = rand() % 1024;
		reference[i] = arr[i];
	}

	// ao dividir o vetor entre as threads, quero
	// garantir que o número de operações realizadas
	// por cada fluxo seja o mais parecido possível
	// para isso, irei calcular o resto da divisao
	// do tamanho do vetor pela numero de threads
	// e usarei esta informação para distribuir os
	// elementos excedentes entre elas
	unsigned remainder = ARR_LEN % NTHREADS;

	arr_heads[0] = &arr[0];
	// gerando as n threads
	for (int i = 0; i < NTHREADS; i++) {
		if (i == 0) {
			// quando o número de threads é maior do que o vetor
			// queremos garantir que aprimeira thread recebe um elemento
			if (NTHREADS > ARR_LEN) {
				arr_tails[i] = arr_heads[i];
				remainder--;
			}
			else {
				// a primeira thread nunca recebe um elemento excedente
				arr_tails[i] = arr_heads[i] + ARR_LEN / NTHREADS - 1;
			}
		}
		else {
			// as demais threads começam com o sucessor
			// imediato do último elemento da thread anterior
			arr_heads[i] = arr_tails[i-1] + 1;
			arr_tails[i] = arr_heads[i] + ARR_LEN / NTHREADS - 1;
			if (remainder) {
				// caso ainda haja excedentes, a thread 
				// atual fica responsável por um deles
				arr_tails[i]++;
				remainder--;
			}
		}

		// gerando estrutura com os argumentos para a nova thread
		arg = malloc(sizeof(array_square_args_t));
		if (!arg) {
			printf("--ERRO: malloc()\n");
			exit(-1);
		}
		arg->head = arr_heads[i];
		arg->tail = arr_tails[i];

		// criando uma nova thread de execução
		if (pthread_create(&tid[i], NULL, array_square, (void *) arg)) {
			printf("--ERRO: pthread_create()\n");
			exit(-1);
		}
	}

	// esperando o término de todas as threads
	for (int i = 0; i < NTHREADS; i++)
		pthread_join(tid[i], NULL);

	for (int i = 0; i < ARR_LEN; i++) {
		if (reference[i] * reference[i] != arr[i]) {
			printf("--ERRO: Resultado incorreto para elemento #%d --> %d*%d != %d\n", 
					i, reference[i], reference[i], arr[i]);
			err_flag++;
		}
	}

	if (err_flag)
		printf("Numero de elementos incorretos: %d\n", err_flag);
	else
		printf("Operação concluída com sucesso.\n");

	return 0;
}

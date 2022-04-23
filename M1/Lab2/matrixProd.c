/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 2 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Implementação e avaliação de aplicações concorrentes */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define DEFAULT_M_SIZE   4 // tamanho padrão das matrizes
#define DEFAULT_NTHREADS 4 // número padão de threads

/* 
 * Estrutura para agrupar os argumentos esperados
 * pela função 'product'. A função precisa receber: 
 *  - O tamanho das matrizes
 *  - O número de linhas a serem calculadas
 *  - Um vetor com cada linha a ser calculada
 * 
 */
typedef struct {
	int * rows;
	int size;
    int nrows;
} product_args_t;

float * a_matrix;
float * b_matrix;
float * res_matrix_seq; // matriz de resultado sequencial
float * res_matrix; // matriz de resultado sequencial

float * alloc_matrix(float **, unsigned); // alocar matrizes
void display_matrix(float *, unsigned); // exibir matrizes
void seq_product(unsigned); // produto sequencial
void * product(void *); // produto concorrente

int main(int argc, char * argv[]) {
    pthread_t * tid;
    product_args_t * args;
    unsigned nthreads;
    unsigned size;

    // verificação de entradas
    switch (argc)
    {
        case 1:
            // Caso o programa seja chamado sem parametros, 
            // usarei um tamanho padrão paras as matrizes 
            // e um número padrão de threads.
            size = DEFAULT_M_SIZE;
            nthreads = DEFAULT_NTHREADS;
            break;
        case 2:
            // Caso o programa seja chamado com apenas um 
            // parametro, ele será o tamanho das as matrizes 
            // e usarei um número padrão de threads.
            size = atoi(argv[1]);
            nthreads = DEFAULT_NTHREADS;
            break;
        case 3:
            // Caso o programa seja chamado os dois argumentos  
            // o primeiro sera o tamanho das as matrizes, e o 
            // segundo o número de threads, exceto quando o 
            // número de threads solicitado for maior que o 
            // tamanho das matrizes, neste caso o número de 
            // threads será limitado ao tamanho.
            size = atoi(argv[1]);
            nthreads = atoi(argv[2]) > size ? size : atoi(argv[2]);
            break;
        default:
            puts("--ERRO: Numero de parametros incorreto!\n");
            puts("Forma de uso: ./<app> <matrix-size*> <number-of-threads*>\n");
            puts("Parametros seguidos de '*' sao opcionais...\n");
            exit(-1);
            break;
    }

    // alocando memória para as matrizes e threads
    alloc_matrix(&a_matrix, size);
    alloc_matrix(&b_matrix, size);
    alloc_matrix(&res_matrix_seq, size);
    alloc_matrix(&res_matrix, size);
    tid = malloc(sizeof(product_args_t) * nthreads);
    if (!tid) {
        printf("--ERRO: malloc()\n");
        exit(-2);
    }

    // inicialização
    srand(time(NULL));
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            // inicializando matrizes 'a' e 'b' com números 
            // aleatórios entre 0 e 10
            *(a_matrix + row * size + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(b_matrix + row * size + col) = (float) rand() / (float) (RAND_MAX / 10.0);
            *(res_matrix_seq + row * size + col) = 0.0;
            *(res_matrix + row * size + col) = 0.0;
        }
    }

    // produto sequencial
    seq_product(size);

    // produto concorrente
	// Optei por fornecer a cada thread a tarefa de calcular
	// um certo número de linhas da matriz resultante, já que 
    // estou limitando o número máximo de threads com base nas 
    // dimensões das matrizes. Para o caso onde o número de 
    // threads é menor que a dimensão das matrizes, usarei uma 
    // abordagem similar a que fiz no primeiro laboratório, 
    // distribuirei as linhas restantes dentre as threads. Assim, 
    // algumas threads serão responsáveis por calcular uma linha 
    // adicional.
    unsigned remainder = size % nthreads;
    int curr_row = 0;
	// gerando as n threads
	for (int i = 0; i < nthreads; i++) {
        // gerando estrutura com os argumentos para a nova thread
		args = malloc(sizeof(product_args_t));
		if (!args) {
			printf("--ERRO: malloc()\n");
			exit(-2);
		}

        // as linhas da matriz resultante são divididas igualmente entre as threads;
        // quando a divisão não é exata, algumas threads recebem uma linha adicional
        args->nrows = size / nthreads + (remainder ? 1 : 0);
		args->size = size;
		args->rows = malloc(sizeof(int) * args->nrows);
		if (!args->rows) {
			printf("--ERRO: malloc()\n");
			exit(-2);
		}

        // determinando por quais linhas a thread será responsável
        for (int j = 0; j < args->nrows; j++) {
            *(args->rows+j) = curr_row++;
        }

        if (remainder)
            remainder--;

		// criando uma nova thread de execução
		if (pthread_create(tid + i, NULL, product, (void *) args)) {
			printf("--ERRO: pthread_create()\n");
			exit(-3);
		}
	}

    // esperando as threads
    for (int i = 0; i < nthreads; i++) {
        pthread_join(*(tid + i), NULL);
    }

    // exibindo matrizes
    puts("matriz a");
    display_matrix(a_matrix, size);
    puts("\nmatriz b");
    display_matrix(b_matrix, size);
    puts("\nmatriz resultado sequencial");
    display_matrix(res_matrix_seq, size);
    puts("\nmatriz resultado concorrente");
    display_matrix(res_matrix, size);

    // liberando memória
    free(a_matrix);
    free(b_matrix);
    free(res_matrix_seq);
    free(res_matrix);
    free(tid);

    return 0;
}

/* 
 * Esta função aloca o espaço necessário para uma matriz.
 * Entradas esperadas:
 *   M --> O ponteiro para uma variavel que guarda um ponteiro para float
 *   size --> O tamanho da matrize
 */
float * alloc_matrix(float ** M, unsigned size) {
    *M = malloc(sizeof(float) * size * size);
    if (!*M) {
        printf("--ERRO: malloc()\n");
        exit(-2);
    }

    return *M;
}

/* 
 * Esta função exibe uma matriz.
 * Entradas esperadas:
 *   M --> O ponteiro para uma matriz
 *   size --> O tamanho da matrize
 */
void display_matrix(float * M, unsigned size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%3.2f ", *(M + row * size + col));
        }
        printf("\n");
    }
}

/* 
 * Esta função calcula o produto entre duas matrizes de 
 * forma sequencial, salvando o resultado em uma terceira. 
 * É esperado que as matrizes fornecidas sejam quadradas.
 * Entradas esperadas:
 *   size --> O tamanho das matrizes
 */
void seq_product(unsigned size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            float sum = 0.0;
            for (int k = 0; k < size; k++) {
                sum += *(a_matrix + i*size + k) * *(b_matrix + k*size + j);
            }
            *(res_matrix_seq + i*size + j) = sum;
        }
    }
}

/* 
 * Esta função calcula o produto entre duas matrizes de 
 * forma concorrente, salvando o resultado em uma terceira 
 * matriz. As matrizes usadas são quadradas e estão declaradas
 * no escopo global.
 * Entradas esperadas:
 *   size  --> O tamanho das matrizes
 *   nrows --> O número de linhas
 *   rows  --> As linhas
 */
void * product(void * arg) {
    product_args_t * args = (product_args_t *) arg;
    int * curr_row = args->rows;

    // calculando todas as linhas sob responsabilidade da thread
    while (args->nrows) {
        for (int i = 0; i < args->size; i++) {
            float sum = 0;
            for (int j = 0; j < args->size; j++) {
                sum += *(a_matrix + *curr_row*args->size + j) * *(b_matrix + j*args->size + i);
            }
            *(res_matrix + *curr_row * args->size + i) = sum;
        }
        curr_row++;
        args->nrows--;
    }

    free(args->rows);
    free(arg);

    return NULL;
}

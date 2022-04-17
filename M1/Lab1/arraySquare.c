/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: "Hello World" usando threads em C */

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define ARR_LEN			10

int main(void) {
	int arr[ARR_LEN];

	srand(time(NULL));
	for (int i = 0; i < ARR_LEN; i++) {
		arr[i] = rand() % 10;
	}

	printf("vetor:\t");
	for (int i = 0; i < ARR_LEN; i++) {
		printf("%d ", arr[i]);
	}
	puts("");

	return 0;
}

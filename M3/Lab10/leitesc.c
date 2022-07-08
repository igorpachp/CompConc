/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 3 - Laboratório: 10 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Exercícios com semaforos */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NESC 2
#define NLEIT 2

sem_t em_e, em_l, escr, leit; // semaforos
int e = 0, l = 0; // filas de ecrita e leitura

void * leitor(void * arg) {
    unsigned long id = (unsigned long) arg;

    while(1) {
        printf("Leitor #%lu quer ler...\n", id);
        sem_wait(&leit); // verificando semaforo de leitores
        sem_wait(&em_l); // mutex de leitores
        l++; // entra na fila
        if (l == 1) sem_wait(&escr); // se for o primeiro, verifica se ha escritores ativos
        sem_post(&em_l); // fim mutex leitores
        sem_post(&leit); // libera semaforo de leitores

        printf("Leitor #%lu está lendo...\n", id);
        usleep(100000);

        sem_wait(&em_l); // mutex leitores
        l--; // sai da fila
        if (l == 0) sem_post(&escr); // se for o último leitor a sair da fila, libera escritores
        sem_post(&em_l); // fim mutex leitores
        printf("Leitor #%lu terminou de ler...\n", id);
    }

    return NULL;
}

void * escritor(void * arg) {
    unsigned long id = (unsigned long) arg;

    while(1) {
        // controle da entrada na fila de escrita
        printf("Escritor #%lu quer escrever...\n", id);
        sem_wait(&em_e); // mutex escritor
        e++; // entra na fila
        if (e == 1) sem_wait(&leit); // se for o primeiro, verifica se ha leitores ativos
        sem_post(&em_e); // fim mutex escritor

        // controle da escrita
        sem_wait(&escr); // bloqueia semaforo escritor
        printf("Escritor #%lu esta lendo...\n", id);
        usleep(100000);
        sem_post(&escr); // libera semaforo de escritor

        // controle da saída da fila de escrita
        sem_wait(&em_e); // mutex escritor
        e--; // sai da fila
        if (e == 0) sem_post(&leit); // libera semaforo de leitor
        sem_post(&em_e); // fim mutex escritor
        printf("Escritor #%lu terminou de escrever...\n", id);
    }

    return NULL;
}

int main() {
    pthread_t tid[NLEIT + NESC]; // leitores primeiro, escritores depois

    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);

    for (unsigned long i = 0; i < NLEIT; i++) {
        pthread_create(&tid[i], NULL, leitor, (void *) i);
    }

    for (unsigned long i = NLEIT; i < NESC + NLEIT; i++) {
        pthread_create(&tid[i], NULL, escritor, (void *) (i - NLEIT));
    }

    for (int i = 0; i < NESC + NLEIT; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&em_e);
    sem_destroy(&em_l);
    sem_destroy(&escr);
    sem_destroy(&leit);
    return 0;
}
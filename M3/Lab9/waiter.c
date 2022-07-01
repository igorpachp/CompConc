/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 3 - Laboratório: 9 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Exercício com semáforos */

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define DEFAULT_NTHREADS 5 // número padrão de threads
#define NTASKS 5

void * last_task(void *);
void * middle_task(void *);
void * first_task(void *);

// variáveis globais
int message_count = 0;
const char * message[NTASKS] = {
    "Volte sempre!", 
    "Fique a vontade", 
    "Sente-se por favor", 
    "Aceita um copo dagua?", 
    "Seja bem-vindo!"
};
sem_t cond_middle, cond_last;

void * tasks[NTASKS] = {last_task, middle_task, middle_task, middle_task, first_task};

int main(int argc, char * argv[]) {
    pthread_t tid[DEFAULT_NTHREADS];

    sem_init(&cond_middle, 0, 0);
    sem_init(&cond_last, 0, 0);

    for (long i = 0; i < DEFAULT_NTHREADS; i++) {
        pthread_create(&tid[i], NULL, tasks[i], (void *) i);
    }

    for (int i = 0; i < DEFAULT_NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    sem_destroy(&cond_middle);
    sem_destroy(&cond_last);

    return 0;
}

void * last_task(void * arg) {
    long index = (long) arg;

    sem_wait(&cond_last);
    puts(message[index]);
    message_count++;
    sem_post(&cond_middle);

    return NULL;
}

void * middle_task(void * arg) {
    long index = (long) arg;

    sem_wait(&cond_middle);
    puts(message[index]);
    if (++message_count == 4) sem_post(&cond_last);
    sem_post(&cond_middle);

    return NULL;
}

void * first_task(void * arg) {
    long id = (long) arg;

    puts(message[id]);
    message_count++;
    sem_post(&cond_middle);

    pthread_exit(NULL);
}

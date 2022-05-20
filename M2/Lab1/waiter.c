/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 2 - Laboratório: 1 */
/* Aluno: Igor Pacheco Pereira */
/* DRE: 119052447 */
/* Codigo: Sincronização condicional com variáveis de condição */

#include <stdio.h>
#include <pthread.h>

// #define DEFAULT_ARR_SIZE 10 // tamanho padrão do vetor
#define DEFAULT_NTHREADS 5 // número padrão de threads
#define NTASKS           5 // número de tarefas

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
pthread_mutex_t lock;
// usarei duas condições, uma para as threads que executam
// logo após a primeira e outra para a última thread
pthread_cond_t condition[2];
void * tasks[NTASKS] = {first_task, middle_task, middle_task, middle_task, last_task};

int main(int argc, char * argv[]) {
    pthread_t tid[DEFAULT_NTHREADS];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&condition[0], NULL);
    pthread_cond_init(&condition[1], NULL);

    for (long long i = 0; i < NTASKS; i++) {
        pthread_create(&tid[i], NULL, tasks[i], (void *) i);
    }

    for (int i = 0; i < DEFAULT_NTHREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&condition[0]);
    pthread_cond_destroy(&condition[1]);

    return 0;
}

void * last_task(void * arg) {
    long long index = (long long) arg;

    puts(message[index]);
    pthread_mutex_lock(&lock);
    message_count++;
    pthread_cond_broadcast(&condition[0]);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void * middle_task(void * arg) {
    long long index = (long long) arg;

    pthread_mutex_lock(&lock);
    if (message_count == 0) pthread_cond_wait(&condition[0], &lock);
    puts(message[index]);
    if (++message_count == 4) pthread_cond_signal(&condition[1]);
    pthread_mutex_unlock(&lock);

    return NULL;
}

void * first_task(void * arg) {
    long long index = (long long) arg;

    pthread_mutex_lock(&lock);
    if (message_count != 4) pthread_cond_wait(&condition[1], &lock);
    pthread_mutex_unlock(&lock);
    puts(message[index]);

    return NULL;
}

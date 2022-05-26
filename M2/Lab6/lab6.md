# Questionário - Módulo 2, Laboratório 6

#### Tópico 1 - Pense o que precisa ser feito para dar prioridade para as operações de escrita, como descrito acima.
Pela definição do problema, duas condições podem impedir o escritor de começar a realizar a sua tarefa: outro escritor está escrevendo, ou um leitor está lendo. Por outro lado, os leitores só são impedidos de agir caso algum escritor esteja *escrevendo*, ou seja, ele pode furar a fila caso um escritor esteja esperando sua vez.
Para dar prioridade aos escritores, precisamos apenas fazer com que leitores também sejam impedidos de agir caso haja algum escritor *esperando*. Perceba que os ecritores ainda não podem escrever enquanto os leitores lêem, pois isso poderia gerar erros durante a execução.

#### Tópico 2 - Implemente a solução projetada, alterando o código que implementa o padrão básico leitores/escritores (fornecido junto com esse roteiro).
No código de referência, ao inicializar um escritor, primeiro verificamos se ele *poderia* executar e, apenas depois de decidir bloquear ou não o fluxo, o inserimos na *fila de escrita*.
```C
// CÓDIGO DE REFERÊNCIA

//entrada leitura
void InicLeit (int id) {
    ...
    while(escr > 0) {
        ...
    }
    ...
}

//entrada escrita
void InicEscr (int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    while((leit > 0) || (escr > 0)) {
        printf("E[%d] bloqueou\n", id);
        pthread_cond_wait(&cond_escr, &mutex);
        printf("E[%d] desbloqueou\n", id);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}
```
Minha proposta de mudança consiste em adicionar o escritor a uma *fila de espera* antes de decidir bloquear o fluxo, removendo-o então desta fila quando for desbloqueado, e inserindo-o na *fila de escrita* quando apropriado.
```C
// CÓDIGO PROPOSTO
void InicLeit (int id) {
    ...
    while(escr > 0 || espr > 0) {
        ...
    }
    ...
}

void InicEscr (int id) {
    pthread_mutex_lock(&mutex);
    printf("E[%d] quer escrever\n", id);
    while((leit > 0) || (escr > 0)) {
        printf("E[%d] bloqueou\n", id);
        espr++;
        pthread_cond_wait(&cond_escr, &mutex);
        printf("E[%d] desbloqueou\n", id);
        espr--;
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}
```

#### Tópico 3 - Execute a aplicação **várias vezes** e avalie os resultados obtidos (observe se ocorrem situações onde a prioridade de escrita foi contemplada).
Podemos ver a prioridade de escrita no seguinte trecho da execução:
```
E[2] quer escrever
Escritora 2 esta escrevendo
E[1] quer escrever
E[1] bloqueou
L[1] quer ler
L[1] bloqueou
E[2] terminou de escrever
L[1] desbloqueou
L[1] bloqueou
E[1] desbloqueou
Escritora 1 esta escrevendo
E[1] terminou de escrever
L[1] desbloqueou
Leitora 1 esta lendo
L[1] terminou de ler
```
Neste trecho vemos o **Escritor 2** iniciando sua tarefa de escrita, resultando no bloquei do **Escritor 1** logo em seguida. O **Leitor 1** então é bloqueado, tanto por haver um escritor na fila de espera *(1)* quanto por haver um escritor na fila de escrita *(2)*.

O **Escritor 2** termina sua rotina, **Leitor 1** recebe o sinal primeiro e verifica se pode seguir em frente, mas identifica o **Escritor 1** na fila de espera e se bloqueia novamente.

Finalmente, **Escritor 1** se desbloqueia e termina sua rotina, liberando **Escritor 1**.

Um problema que eu notei com esta abordagem, é que é possível que um escritor fure a *fila de espera*, como na execução a seguir:
```
Leitora 1 esta lendo
E[1] quer escrever
E[1] bloqueou
L[1] terminou de ler
E[2] quer escrever
Escritora 2 esta escrevendo
E[2] terminou de escrever
E[1] desbloqueou
Escritora 1 esta escrevendo
E[1] terminou de escrever
```
Perceba que **Escritor 1** estava na fila de espera, aguardando **Leitor 1** terminar sua rotina, mas por coincidência **Escritor 2**, sem entrar na fila de espera, iniciou a escrita antes que **Escritor 1** tivesse a chance de verificar se poderia executar.

Considerando este comportamento, pensei em modificar o código de escritor para que considerasse também a fila de espera, da seguinte forma:

```C
void InicEscr (int id) {
    ...
    while((leit > 0) || (escr > 0) || (espr > 0)) {
        ...
    }
}
```
Percebi que esta alteração causa deadlock em casos onde os dois escritores são inseridos na fila de espera, por conta de identificarem um leitor no meio da leitura por exemplo, já que um ficaria esperando o outro sair da esperar e entrar em escrita e no fim nenhum dos dois escreve.

Por sorte, nossa aplicação não exige que os escritores escrevam por ordem de chegada. Se este fosse o caso, penso que talvez precisariamos de uma estrutura de dados como uma *fila* propriamente dita, ou alguma verificação condicional mais específica que não fui capaz de identificar.

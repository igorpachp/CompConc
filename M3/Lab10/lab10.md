# Questionário - Módulo 3, Laboratório 10

## Atividade 1
O algoritmo abaixo implementa uma solução para o problema dos leitores e escritores (mais de um leitor pode ler ao mesmo tempo; apenas um escritor pode escrever de cada vez e nenhum leitor pode estar lendo) com prioridade para escrita (sempre que há um escritor esperando, o acesso para novos leitores é bloqueado até e que o escritor seja atendido).

<table>
<tr>
<th> Leitores </th>
<th> Escritores </th>
</tr>
<tr>
<td>

```C
while(1) {
    sem_wait(&leit);
    sem_wait(&em_l); l++;
    if(l==1) sem_wait(&escr);
    sem_post(&em_l);
    sem_post(&leit)
    //le...
    sem_wait(&em_l); l--;
    if(l==0) sem_post(&escr);
    sem_post(&em_l);
}
```

</td>
<td>

```C
while(1) {
    sem_wait(&em_e); e++;
    if(e==1) sem_wait(&leit);
    sem_post(&em_e);
    sem_wait(&escr);
    //escreve...
    sem_post(&escr);
    sem_wait(&em_e); e--;
    if(e==0) sem_post(&leit);
    sem_post(&em_e);
}
```

</td>
</tr>
</table>

#### A - Quais devem ser os valores iniciais dos semáforos (em_e, em_l, escr, leit) para que o algoritmo funcione corretamente?
Os semáforos *em_e* e *em_l* são usados para exclusão mútua, logo precisão ser iniciados com valor 1, de outra forma a primeira thread a utilizar *sem_wait(&em_e)* ou *sem_wait(&em_l)* pode não conseguir decrementar seu valor e ficar tavada, no caso de serem iniciadas com 0, ou múltiplas threads podem acessar o trecho que requer exclusão, caso sejam iniciadas com valores maiores que 1.

Como não temos uma regra que diz qual tipo de thread precisamos executar primeiro, nossa implementação deve permitir que qualquer thread possa ser escolhida ao iniciar a execução, assim ambos os semáforos *escr* e *leit* devem ser inicializados com valor 1.

#### B - É possível simplificar esse código (reduzir o uso dos mecanismos de sincronização)?
Não.

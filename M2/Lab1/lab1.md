# Questionário - Módulo 2, Laboratório 1

## Atividade 1
#### Tópico 2 - Execute o programa **várias vezes**. O log de execução impresso na tela foi sempre o esperado? A condição lógica da aplicação foi atendida em todas as execuções?
Sim, sempre houve a impressão de dois "HELLO" antes que um "BYE" fosse impresso.

#### Tópico 3 - Agora altere o numero de threads A para 1. O que vai ocorrer na execução? O programa vai terminar? Por que?
O programa não irá terminar pois a thread B ficará presa esperando infinitamente para que a condição de desbloqueio seja atendida, o que não vai acontecer já que será feito apenas um incremento na variável condicional global.

#### Tópico 4 e 5 - Altere o numero de threads A de volta para 2. Agora altere o numero de threads B para 2 e faça as correções necessárias no código para que a aplicação continue funcionando.
Para que o programa permita duas threads B e duas threads A, devemos aumentar o número global de threads para 4, e devemos também inicializar mais uma thread B dentro da função principal. Com isto feito, o programa parece funcionar corretamente na maior parte das execuções, mas é possivel notar que, caso ambas as threads B sejam bloqueadas antes de x ser incrementado à 2, o programa necessariamente entrará em deadlock, já que as threads A apenas liberam uma thread B para a execução, visto que elas utilizam *signal* e não *broadcast*. Logo para que a condição lógica do programa seja respeitada, devemos alterar a função A para que realize um *broadcast* ao invés de *signal*.

## Atividade 2
#### Tópico 2 - Execute o programa *várias vezes*. O log de execução impresso na tela foi sempre o esperado? A condição lógica da aplicação foi atendida em todas as execuções?
Sim, a impressão de abos os "BYE" ocorreram sempre após da impressão do "HELLO".

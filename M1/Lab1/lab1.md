# Questionário - Laboratório 1

#### 1 - Abra o arquivo **hello.c**, execute o programa *varias vezes* e observe os resultados impressos na tela. **Há mudanças na ordem de execução das threads? Por que isso ocorre?**
Sim, há mudança na ordem de execução. Isto ocorre por que não há como garantir que as threads terminarão seus fluxos na mesma ordem que foram criadas.

#### 2 - Abra o arquivo **hello_arg.c**, execute o programa *varias vezes* e observe os resultados impressos na tela. **Qual foi a diferença em relação ao programa anterior?**
A diferença é que agora sabemos o ordem que as threads terminam, graças à passagem do id da thread por meio de um argumento.

#### 3 - Abra o arquivo **hello_args.c**, execute o programa *varias vezes* e observe os resultados impressos na tela. **O programa funcionou como esperado?**
Acredito que sim, ambos os argumentos puderam ser acessados pelas threads, apesar de ainda termos a questão da thread principal se encerrar antes que algumas das outras threads terminem.

#### 4 - Abra o arquivo **hello_join.c**, execute o programa *varias vezes* e observe os resultados impressos na tela. **O que aconteceu de diferente em relação às versões/execuções anteriores?**
Agora independente da ordem em que as threads terminam, thread principal aguarda pacientemente pelo termino de todas antes de se encerrar.

# Questionário - Módulo 2, Laboratório 7

## Atividade 1
#### 3 - Execute o programa **várias vezes** (fazendo java HelloThread) e observe os resultados impressos na tela. **Há mudanças na ordem de execução das threads? Por que isso ocorre?**
Sim, há mudança na ordem de execução. Isto ocorre por que não há como garantir que as threads terminarão seus fluxos na mesma ordem que foram criadas.

#### 4 - Descomente as linhas 43-46 e compile o programa novamente.
#### 5 - Execute o programa **várias vezes** e observe os resultados impressos na tela. **Qual alteração na execução da aplicação pode ser observada e por que ela ocorre?**
Com as linhas comentadas, em algumas execuções, o fluxo principal pode terminar antes que todas as threads completem suas rotinas, resultando na impressão da palavra "Terminou" em momentos aleatórios. Já com as linhas descomentadas, "Terminou" sempre é impressa após a execução completa de todos os outros fluxos.

## Atividade 3
#### 1 - Leia o programa para entender o que ele faz **(acompanhe a explanação da professora na vídeo-aula). Qual e a seção crítica do codigo?** Qual saída é esperada para o programa (valor final de **s**)?
A seção crítica é o incremento da variavel *r* na classe *S*. Espera-se ter como saída o número 200000.
#### 1 - Compile o programa, execute-o **várias vezes** e observe os resultados impressos na tela. **Os valores impressos foram sempre o valor esperado? Por que?**
Não, porque não havia nenhum controle no acesso a variavel compartilhada, logo algumas vezes ambas as threads acabavam modificando e o valor da variavel ao mesmo tempo, sobrescrevendo a operação realizada uma pela outra.

## Atividade 4
#### 1 - Compile o programa, execute-o **várias vezes** e observe os resultados impressos na tela. **Os valores impressos foram sempre o valor esperado? Por que?**
Sim. Pois dessa vez há garantia de que apenas um fluxo acessa a variavél por vez.

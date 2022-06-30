#include <math.h>
#include "sequencial.h"

/* 
 * Esta função utiliza o método dos trapézios para calcular a integral 
 * de uma função discreta à partir dos vetores de coordenadas de forma 
 * sequencial.
 * 
 * Entradas esperadas:
 *   x -> um ponteiro para os pontos em um intervalo no domínio ordenado de forma crescente;
 *   y -> um ponteiro para os pontos da imagem correspondentes aos ponto do domínio recebidos em 'x';
 *   size -> o número de coordenadas em 'x' e 'y';
 * 
 * Retorna:
 *   sum --> O somatório da área de todos os trapézios no intervalo;
 */
double integral_discreta_sequencial(double * x, double * y, unsigned size) {
    double sum = 0;

    for (int i = 0; i < size - 1; i++) {
        // área do trapézio, (B + b) * h/2, neste caso
        // temos B = y[i+1], b = y[i], h = x[i+1] - x[i]
        sum += (y[i] + y[i + 1]) * (x[i + 1] - x[i]);
    }

    // efetuamos a divisão fora do laço para economizar algumas operações
    return sum / 2;
}

/* 
 * Esta função utiliza o método dos trapézios para calcular, de forma 
 * sequencial, a integral de uma função continua à partir de um 
 * intervalo fechado no domínio da função e o número de divisões 
 * desejadas neste intervalo.
 * 
 * Entradas esperadas:
 *   function -> uma função matemática genérica que receba e rotorne um 'double';
 *   intervals -> o número de divisões desejado no intervalo;
 *   lower_edge -> o limite inferior do intervalo, incluso;
 *   upper_edge -> o limite superior do intervalo, incluso;
 * 
 * Retorna:
 *   sum --> O somatório da área de todos os trapézios no intervalo;
 */
double integral_continua_sequencial(double (*function)(double), unsigned intervals, double lower_edge, double upper_edge) {
    // calculo da altura do trapézio
    double distance = (upper_edge - lower_edge) / intervals;

    // no calculo da área do trapézio para uma função discreta, 
    // veremos que ao calcular o trapézio de número 'n' teremos 
    // Sn = ( f(n) + f(n+1) ) * h / 2
    // e para o calculo do trapézio 'n+1'
    // Sn+1 = ( f(n+1) + f(n+2) ) * h / 2
    // perceba que uma das bases é considerada duas vezes para 
    // trapézios consecutivos, utilizaremos este fato para 
    // economizar algumas iterações no laço, já que o calculo de f(n)
    // pode ser custoso, dependendo da função recebida
    double sum = function(lower_edge) + function(upper_edge);
    // perceba também que f(a) e f(b), 'a' e 'b' limites do intervalo, 
    // serão consideradas uma única vez, logo são adicionadas fora do laço
    for (unsigned i = 1; i < intervals; i++) {
        // calculamos f(n) uma única vez para todo n, a < n < b
        // e múltiplicamos por 2 para evitar calcular duas vezes 
        // o mesmo resultado
        sum += 2 * function(lower_edge + distance * i);
    }

    // ainda economizando operações, visto que a distância entre os pontos 
    // é fixa na implementação contínua, podemos executar tanto a divisão
    // quanto a multiplicação após a execução do laço
    return sum * distance / 2;
}

/* 
 * Esta função utiliza o método dos trapézios para calcular, de forma 
 * sequencial, a integral de uma função contínua à partir de um intervalo 
 * fechado no domínio da função.
 * Esta função também garante que o resultado esteja dentro de um intervalo
 * de precisão determinado atravéz de um limite superior para a segunda 
 * derivada da função e uma tolerância, ambas informações recebidas também 
 * como argumento.
 * 
 * Entradas esperadas:
 *   function -> uma função matemática genérica que receba e rotorne um 'double';
 *   lower_edge -> o limite inferior do intervalo, incluso;
 *   upper_edge -> o limite superior do intervalo, incluso;
 *   precision -> a tolerância para o erro do resultado calculado, geralmente uma potência negativa de 10;
 *   second_derivative_ceil -> o teto para a segunda derivada no intervalo;
 * 
 * Retorna:
 *   sum --> O somatório da área de todos os trapézios no intervalo;
 */
double integral_continua_com_precisao_sequencial(double (*function)(double), double lower_edge, double upper_edge, double precision, double second_derivative_ceil) {
    unsigned intervals = ceil(sqrt(pow(upper_edge - lower_edge, 3) * second_derivative_ceil / (12 * precision)));

    // após determinar a quantidade de intervalos necessária para atender 
    // a precisao desejada, podemos reutilizar o código de uma integral continua
    return integral_continua_sequencial(function, intervals, lower_edge, upper_edge);
}

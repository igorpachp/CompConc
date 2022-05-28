#include <stdio.h>
#include <math.h>
#include "sequencial.h"

double reference_function(double x) {
    return 0.5 * sin(3 * x) + 3 * cos(x / 5);
}

int main() {
    printf("%lf\n", integral_continua(reference_function, 10, -1, 1));
    return 0;
}
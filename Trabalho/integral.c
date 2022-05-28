#include <stdio.h>
#include <math.h>
#include "sequencial.h"

double reference_function(double x) {
    return 0.5 * sin(3 * x) + 3 * cos(x / 5);
}

int main() {
    double x[] = {-1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
    double y[sizeof(x) / sizeof(double)];

    for (int i = 0; i < sizeof(x) / sizeof(double); i++) {
        y[i] = reference_function(x[i]);
    }

    printf("%lf\n", integral_discreta(x, y, sizeof(x) / sizeof(double)));
    printf("%lf\n", integral_continua(reference_function, 10, -1, 1));
    printf("%lf\n", integral_continua_com_precisao(reference_function, -1, 1, 0.01, 4.62));

    return 0;
}
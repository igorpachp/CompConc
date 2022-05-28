#include "sequencial.h"

double integral_discreta(double * x, double * y, unsigned size) {
    double sum = 0;

    for (int i = 0; i < size - 1; i++) {
        sum += (y[i] + y[i + 1]) * (x[i + 1] - x[i]);
    }

    return sum / 2;
}

double integral_continua(double (*function)(double), unsigned intervals, double lower_edge, double upper_edge) {
    double distance = (upper_edge - lower_edge) / intervals;

    double sum = function(lower_edge) + function(upper_edge);
    for (int i = 1; i < intervals; i++) {
        sum += 2 * function(lower_edge + distance * i);
    }
    return sum * distance / 2;
}

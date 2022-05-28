#include "sequencial.h"

double integral_continua(double (*function)(double), unsigned intervals, double lower_edge, double upper_edge) {
    double distance = (upper_edge - lower_edge) / intervals;

    double sum = function(lower_edge) + function(upper_edge);
    for (int i = 1; i < intervals; i++) {
        sum += 2 * function(lower_edge + distance * i);
    }
    return sum * distance / 2;
}

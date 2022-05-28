#ifndef __SEQUENCIAL_H__
#define __SEQUENCIAL_H__

double integral_discreta(double *, double *, unsigned);
double integral_continua(double (*)(double), unsigned, double, double);
double integral_continua_com_precisao(double (*)(double), double, double, double, double);

#endif

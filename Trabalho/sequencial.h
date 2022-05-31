#ifndef __SEQUENCIAL_H__
#define __SEQUENCIAL_H__

double integral_discreta_sequencial(double *, double *, unsigned);
double integral_continua_sequencial(double (*)(double), unsigned, double, double);
double integral_continua_com_precisao_sequencial(double (*)(double), double, double, double, double);

#endif

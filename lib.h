#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define EPS pow (10, -15)
#define pi  3.14159265359

int    func1 (void);
double func2 (void);
double func3 (void);

double func21 (void);
double func22 (void);
double func23 (void);

double mf (double);
void   func31 (void);

void func41 (void);
void func42 (void);

void func51 (void);

double rd (FILE*);
void   RK (double, double*, double*, double, unsigned int, double**, double**,
           double f (double, double, double), double g (double, double, double),
           bool);
void   func61 (void);

void func71 (void);

void func81 (void);

// void func91 (void);
#endif

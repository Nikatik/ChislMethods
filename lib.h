#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <math.h>
#include <quadmath.h>
#include <stdbool.h>
#include <stdio.h>

#define EPSo pow (10, -15)
#define EPS  (__float128) pow (10, -34)
#define pi   3.1415926535897932384626433

int        func1 (void);
__float128 func2 (void);
__float128 func3 (void);

double func21 (void);
double func22 (void);
double func23 (void);

__float128 mf (__float128);
void       func31 (void);

void func41 (void);
void func42 (void);

void func51 (void);

__float128 rd (FILE*);
void RK (double, double*, double*, double, unsigned int, double**, double**,
         double f (double, double, double), double g (double, double, double),
         bool);
void RKq (__float128 t, __float128* x0, __float128* y0, __float128 h,
          unsigned int s, __float128** k, __float128** cab,
          __float128 f (__float128, __float128, __float128),
          __float128 g (__float128, __float128, __float128), bool check);
void func61 (void);

void func71 (void);

void func81 (void);

// void func91 (void);
#endif

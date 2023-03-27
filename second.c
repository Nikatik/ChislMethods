#include "lib.h"

double f (double x);

double func21 (void)
{
    int    n = 31;
    double x = log (7. / 6.);
    for (int i = 1; i <= n; i++) { x = 1. / i - 6. * x; }
    return x;
}

double func22 (void)
{
    int    n = 31;
    double x = 0;
    for (int i = 60; i >= n; i--) { x = (1. / i - x) / 6.; }
    return x;
}

double f (double x) { return pow (x, 31) / (x + 6.); }

double func23 (void)
{
    double res = 0;
    double x   = 0;
    while (x < 1)
    {
        res += (f (x) + f (x + pow (10., -3))) * pow (10., -3) * 0.5;
        x += pow (10., -3);
    }
    return res;
}

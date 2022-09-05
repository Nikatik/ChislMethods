#include "lib.h"
double el1 (double, double);
double dmf (double);
double el2 (double, double);

double el1 (double x, double h)
{
    double res = x;
    for (int i = 0; i < 10 / h; i++)
    {
        res += h * h * i;
    }
    return res;
}

void func41 (void)
{
    double x    = 0;
    double h    = 1;
    double temp = pow (10., 2) / 2;
    printf ("Func 4.a:\n  h     |      R1");
    while (h > pow (10., -5))
    {
        printf ("\n%.0e\t|%13.4e", h, temp - el1 (x, h));
        h *= 0.1;
    }
    printf ("\n");
}

double dmf (double x)
{
    return pow (x, 4) - 51 * pow (x, 2) + 32 - 3 * exp (3 * x) + 1 / (x + 1);
}

double el2 (double x, double h)
{
    double res = x;
    for (int i = 0; i < 3 / h; i++)
    {
        res += h * dmf (h * i);
    }
    return res;
}

void func42 (void)
{
    double x    = 9;
    double h    = 1;
    double temp = mf (3);
    printf ("\nFunc 4.b:\n  h     |      R1");
    while (h > pow (10., -5))
    {
        printf ("\n%.0e\t|%13.4e", h, temp - el2 (x, h));
        h *= 0.1;
    }
    printf ("\n");
}

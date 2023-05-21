#include "lib.h"
#define max1 10
#define max2 5

double el1 (double x, double h);
double dmf (double x);
double el2 (double x, double h);

double el1 (double x, double h)
{
    double res = x;
    for (int i = 1; i * h < max1;)
    {
        res += h * h * i;
        i++;
        /*if(i*h>max1)
            printf("\nlast point %e\n", i*h);*/
    }
    return res;
}

void func41 (void)
{
    double x    = 0;
    double h    = 1;
    double temp = pow (max1, 2) / 2;
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
    for (int i = 1; i * h < max2; i++) { res += h * dmf (h * i); }
    return res;
}

void func42 (void)
{
    double x    = 9;
    double h    = 1;
    double temp = (double) mf (max2);
    printf ("\nFunc 4.b:\n  h     |      R1");
    while (h > pow (10., -5))
    {
        printf ("\n%.0e\t|%13.4e", h, temp - el2 (x, h));
        h *= 0.1;
    }
    printf ("\n");
}

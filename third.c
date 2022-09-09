#include "lib.h"

double mf (double x)
{
    return pow (x, 5) / 5 - 17 * pow (x, 3) + 32 * x - exp (3 * x) +
           log (x + 1) + 9;
}

void func31 (void)
{
    double x = 0;
    double h;
    printf ("Func 3:\n  h     |      R1");
    for (int i = 1; i < 21; i++)
    {
        h = pow (10., -i) / 2.;
        printf ("\n%.0e\t|%13.4e",
                2 * h,
                fabs (30. - (mf (x + h) - mf (x - h)) / (2 * h)));
    }
    printf ("\n");
}

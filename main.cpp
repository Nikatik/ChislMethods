#include <math.h>
#include <stdio.h>

unsigned int func1 (void)
{
    double eps = 1;
    _Pragma ("GCC diagnostic push");
    _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
    for (unsigned int i = 0;; i++)
    {
        if (1. + eps == 1.)        //-Wfloat-equal
        {
            _Pragma ("GCC diagnostic pop");
            return i;
        }
        eps = ldexp (1., -i);
    }
    return 0;
}

double func2 (void)
{
    double x = 0.;
    _Pragma ("GCC diagnostic push");
    _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
    for (unsigned int i = 0; x + 1. != x; i++)
        x += ldexp (2., i / 20);

    _Pragma ("GCC diagnostic pop");
    return x;
}

double func3 (void)
{
    double x = 0.;
    _Pragma ("GCC diagnostic push");
    _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
    for (unsigned int i = 0; x + pow (10., 20) != x; i++)
        x += ldexp (2., i / 20);
    _Pragma ("GCC diagnostic pop");
    return x;
}

double func21 (void)
{
    int n    = 31;
    double x = log (7. / 6.);
    for (int i = 1; i <= n; i++)
    {
        x = 1. / i - 6. * x;
    }
    return x;
}

double func22 (void)
{
    int n    = 31;
    double x = log (6.00000001 / 6.);
    for (int i = 60; i >= n; i--)
    {
        x = (1. / i - x) / 6.;
    }
    return x;
}

double f (double x)
{
    return pow (x, 31) / (x + 6.);
}

double func23 (void)
{
    double res = 0;
    double x   = 0;
    while (x < 1)
    {
        res += (f (x) + f (x + pow (10., -3))) * pow (10., -3);
        x += pow (10., -3);
    }
    return res;
}

double mf (double x)
{
    return pow (x, 5) / 5 - 17 * pow (x, 3) + 32 * x - exp (3 * x) +
           log (x + 1) + 9;
}

void func31 (void)
{
    double x = 0;
    double h;
    printf ("\nFunc 3:\n  h     |      R1");
    for (int i = 1; i < 21; i++)
    {
        h = pow (10., -i) / 2.;
        printf ("\n%.0e\t|%13.4e",
                2 * h,
                fabs (30. - (mf (x + h) - mf (x - h)) / (2 * h)));
    }
    printf ("\n");
}

int main (void)
{
    printf ("Func 1.a:%13.4e\nFunc 1.b:%13.4e\nFunc 1.c:%13.4e\n",
            ldexp (1., -func1()),
            func2(),
            func3());
    printf ("\nFunc 2.a:%13.4e\nFunc 2.b:%13.4e\nFunc 2.c:%13.4e\n",
            func21(),
            func22(),
            func23());
    func31();
    return 0;
}

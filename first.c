#include "lib.h"

int func1 (void)
{
    double eps = 1;
    _Pragma ("GCC diagnostic push");
    _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
    for (int i = 0;; i++)
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
    for (int i = 0; x + 1. != x; i++) {
        x += ldexp (2., i / 20);
}
    _Pragma ("GCC diagnostic pop");
    return x;
}

double func3 (void)
{
    double x = 0.;
    _Pragma ("GCC diagnostic push");
    _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
    for (int i = 0; x + pow (10., 20) != x; i++) {
        x += ldexp (2., i / 20); }
    _Pragma ("GCC diagnostic pop");
    return x;
}


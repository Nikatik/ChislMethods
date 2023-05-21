#include "lib.h"
#include <quadmath.h>

int func1 (void)
{
    __float128 eps = 1;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    for (int i = 0;; i++)
    {
        if ((__float128) 1. + eps == (__float128) 1.)        //-Wfloat-equal
        {
#pragma GCC diagnostic pop
            return i;
        }
        eps = ldexpq ((__float128) 1., -i);
    }
    return 0;
}

__float128 func2 (void)
{
    __float128 x = (__float128) 0.;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    for (int i = 0; x + (__float128) 1. != x; i++)
    {
        x += ldexpq ((__float128) 2., i / 20);
    }
#pragma GCC diagnostic pop
    return x;
}

__float128 func3 (void)
{
    __float128 x = (__float128) 0.;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    for (int i = 0; x + powq ((__float128)10., (__float128)20) != x; i++)
    {
        x += ldexpq ((__float128) 2., i / 20);
    }
#pragma GCC diagnostic pop
    return x;
}

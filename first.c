#include "lib.h"

int func1 (void)
{
    __float128 eps = 1;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    for (int i = 0;; i++)
    {
        if (1 + eps == 1)        //-Wfloat-equal
        {
#pragma GCC diagnostic pop
            return i;
        }
        eps = ldexpq (1.Q, -i);
    }
    return 0;
}

__float128 func2 (void)
{
    __float128 x = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    for (int i = 0; x + 1 != x; i++) { x += ldexpq (2.Q, i / 20); }
#pragma GCC diagnostic pop
    return x;
}

__float128 func3 (void)
{
    __float128 x = 0;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    for (int i = 0; x + powq (10.Q, 20) != x; i++)
    {
        x += ldexpq (2.Q, i / 20);
    }
#pragma GCC diagnostic pop
    return x;
}

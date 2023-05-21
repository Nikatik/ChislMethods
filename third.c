#include "lib.h"

__float128 mf (__float128 x)
{
    return powq (x, 5) / 5 - 17 * powq (x, 3) + 32 * x - expq (3 * x) +
           logq (x + 1) + 9;
}

void func31 (void)
{
    __float128 x = 0;
    __float128 h;
    printf ("Func 3.1:\n  h     |      R1");
    for (int i = 1; i < 25; i++)
    {
        h = powq (10.Q, -i);
        printf ("\n%.0Le\t|%13.4Le", (long double) h,
                (long double) fabsq (30 - (mf (x + h) - mf (x)) / h));
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.2:\n  h     |      R1");
    for (int i = 1; i < 25; i++)
    {
        h = powq (10.Q, -i);
        printf ("\n%.0Le\t|%13.4Le", (long double) h,
                (long double) fabsq (30 - (mf (x + h) - mf (x - h)) / (2 * h)));
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.3:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 30; i++)
    {
        h = powq (2.Q, -i);
        printf ("\n%.3Le\t|%13.4Le\t|\t%d", (long double) h,
                (long double) fabsq (30 -
                                     (2 * mf (x + h) - 2 * mf (x - h) -
                                      mf (x + 2 * h) / 4 + mf (x - 2 * h) / 4) /
                                         (3 * h)),
                -i);
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.4:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 30; i++)
    {
        h = powq (2.Q, -i);
        printf ("\n%.3Le\t|%13.4Le\t|\t%d", (long double) h,
                (long double) fabsq (
                    30 - (3 * mf (x + h) - 3 * mf (x - h) -
                          3 * mf (x + 2 * h) / 5 + 3 * mf (x - 2 * h) / 5 +
                          mf (x + 3 * h) / 15 - mf (x - 3 * h) / 15) /
                             (4 * h)),
                -i);
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.5:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 30; i++)
    {
        h = powq (2.Q, -i);
        printf ("\n%.3Le\t|%13.4Le\t|\t%d", (long double) h,
                (long double) fabsq (
                    30 - (4 * mf (x + h) - 4 * mf (x - h) - mf (x + 2 * h) +
                          mf (x - 2 * h) + 4 * mf (x + 3 * h) / 21 -
                          4 * mf (x - 3 * h) / 21 - mf (x + 4 * h) / 56 +
                          mf (x - 4 * h) / 56) /
                             (5 * h)),
                -i);
    }
    printf ("\n");
}

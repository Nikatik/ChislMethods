#include "lib.h"

#include <math.h>

__float128 mf (__float128 x)
{
    return (__float128) powl (x, (__float128) 5) / (__float128) 5. -
           (__float128) 17. * (__float128) powl (x, (__float128) 3) +
           (__float128) 32. * x - (__float128) expl ((__float128) 3. * x) +
           (__float128) logl (x + (__float128) 1) + (__float128) 9.;
}

void func31 (void)
{
    __float128 x = 0;
    __float128 h;
    printf ("Func 3.1:\n  h     |      R1");
    for (int i = 1; i < 25; i++)
    {
        h = (__float128) powl ((__float128) 10., -i);
        printf ("\n%.0Le\t|%13.4Le", (long double) h,
                fabsl ((__float128) 30. - (mf (x + h) - mf (x)) / (h)));
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.2:\n  h     |      R1");
    for (int i = 1; i < 25; i++)
    {
        h = (__float128) powl ((__float128) 10., -i);
        printf ("\n%.0Le\t|%13.4Le", (long double) h,
                fabsl ((__float128) 30. -
                       (mf (x + h) - mf (x - h)) / ((__float128) 2 * h)));
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.3:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 30; i++)
    {
        h = (__float128) powl ((__float128) 2., -i);
        printf ("\n%.3Le\t|%13.4Le\t|\t%d", (long double) h,
                fabsl ((__float128) 30. -
                       ((__float128) 2. * mf (x + h) -
                        (__float128) 2. * mf (x - h) -
                        mf (x + (__float128) 2 * h) / (__float128) 4. +
                        mf (x - (__float128) 2 * h) / (__float128) 4.) /
                           ((__float128) 3 * h)),
                -i);
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.4:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 30; i++)
    {
        h = (__float128) powl ((__float128) 2., -i);
        printf ("\n%.3Le\t|%13.4Le\t|\t%d", (long double) h,
                fabsl ((__float128) 30. -
                       ((__float128) 3. * mf (x + h) -
                        (__float128) 3. * mf (x - h) -
                        (__float128) 3. * mf (x + (__float128) 2 * h) /
                            (__float128) 5. +
                        (__float128) 3 * mf (x - (__float128) 2 * h) /
                            (__float128) 5. +
                        mf (x + (__float128) 3 * h) / (__float128) 15. -
                        mf (x - (__float128) 3 * h) / (__float128) 15.) /
                           ((__float128) 4 * h)),
                -i);
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.5:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 30; i++)
    {
        h = (__float128) powl ((__float128) 2., -i);
        printf (
            "\n%.3Le\t|%13.4Le\t|\t%d", (long double) h,
            fabsl ((__float128) 30. -
                   ((__float128) 4. * mf (x + h) -
                    (__float128) 4. * mf (x - h) - mf (x + (__float128) 2 * h) +
                    mf (x - (__float128) 2 * h) +
                    (__float128) 4. * mf (x + 3 * h) / (__float128) 21. -
                    (__float128) 4. * mf (x - 3 * h) / (__float128) 21. -
                    mf (x + (__float128) 4 * h) / (__float128) 56. +
                    mf (x - (__float128) 4 * h) / (__float128) 56.) /
                       ((__float128) 5 * h)),
            -i);
    }
    printf ("\n");
}

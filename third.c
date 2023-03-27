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
    printf ("Func 3.1:\n  h     |      R1");
    for (int i = 1; i < 16; i++)
    {
        h = pow (10., -i);
        printf ("\n%.0e\t|%13.4e", h, fabs (30. - (mf (x + h) - mf (x)) / (h)));
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.2:\n  h     |      R1");
    for (int i = 1; i < 16; i++)
    {
        h = pow (10., -i);
        printf ("\n%.0e\t|%13.4e", h,
                fabs (30. - (mf (x + h) - mf (x - h)) / (2 * h)));
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.3:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 16; i++)
    {
        h = pow (2., -i);
        printf ("\n%.3e\t|%13.4e\t|\t%d", h,
                fabs (30. - (2. * mf (x + h) - 2. * mf (x - h) -
                             mf (x + 2 * h) / 4. + mf (x - 2 * h) / 4.) /
                                (3 * h)),
                -i);
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.4:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 16; i++)
    {
        h = pow (2., -i);
        printf (
            "\n%.3e\t|%13.4e\t|\t%d", h,
            fabs (30. - (3. * mf (x + h) - 3. * mf (x - h) -
                         3. * mf (x + 2 * h) / 5. + 3 * mf (x - 2 * h) / 5. +
                         mf (x + 3 * h) / 15. - mf (x - 3 * h) / 15.) /
                            (4 * h)),
            -i);
    }
    printf ("\n\n");
    x = 0;
    printf ("Func 3.5:\n      h         |       R1      |    degree (2)");
    for (int i = 1; i < 16; i++)
    {
        h = pow (2., -i);
        printf (
            "\n%.3e\t|%13.4e\t|\t%d", h,
            fabs (30. - (4. * mf (x + h) - 4. * mf (x - h) - mf (x + 2 * h) +
                         mf (x - 2 * h) + 4. * mf (x + 3 * h) / 21. -
                         4. * mf (x - 3 * h) / 21. - mf (x + 4 * h) / 56. +
                         mf (x - 4 * h) / 56.) /
                            (5 * h)),
            -i);
    }
    printf ("\n");
}

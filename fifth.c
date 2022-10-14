#include "lib.h"

void func51 (void)
{
    double h = 1;
    double T = 5 * pi;
    double tmp;

    double x;
    double z;
    double temp;
    double dist;

    for (int i = 1; h > 5 * pow (10., -3); i++)
    {
        printf ("Func 5.%d:\nh = %5.5f\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)\n",
                i,
                h);

        while (T < 1.5 * pow (10., 4) * pi)
        {
            x    = 0.;
            z    = 1.;
            dist = 0.;
            tmp  = h;
            for (; T - dist > EPS;)
            {
                if (dist + h + EPS > T) h = T - dist;
                temp = x;
                x += h * z;
                z -= h * temp;
                dist += h;
                //printf("%.17e\n",z);
            }
            h = tmp;
            _Pragma ("GCC diagnostic push");
            _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
            if (x != x || z != z)
            {
                break;
            }
            _Pragma ("GCC diagnostic pop");

            printf (" %7.0fPi   | %11.3e  | %11.3e  \n",
                    (int)T / pi,
                    x - sin (T),
                    z - cos (T));

            if (T < 9. * pi)
            {
                T += 5 * pi;
                continue;
            }
            if (T > 95000. * pi)
            {
                T += 100000. * pi;
                continue;
            }
            if (h > 0.005 && T > 9500. * pi)
            {
                T += 10000. * pi;
                continue;
            }
            if (h > 0.05 && T > 950. * pi)
            {
                T += 1000. * pi;
                continue;
            }
            T *= 10.;
        }

        T = 5 * pi;
        printf ("\n");
        h *= 0.1;
    }
}

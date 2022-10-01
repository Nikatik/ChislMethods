#include "lib.h"
#include <stdlib.h>

int rs (FILE* inpf)        // reading string with word separation
{
    int count = 0;
    int i     = 0;
    char temp[256];
    temp[0] = '0';

    if (feof (inpf))
    {
        return 0;
    }

    for (;;)
    {
        for (i = 0; i < 256; i++)
        {
            if (fscanf (inpf, "%c", temp + i) <= 0 || temp[i] == ' ' ||
                temp[i] == '\n' || temp[i] == '\0')
            {
                break;
            }
        }
        // printf("\n\nscaned %d\n", i);
        count = i;
        for (; i > 0; i--)
        {
            printf ("%c", temp[count - i]);
        }
        if (count == 256) count--;
        if ((temp[count] == '\n' && count != 0) || feof (inpf))
        {
            printf ("\n");
            break;
        }
        if (count != 0)
        {
            printf (" ");
        }
    }
    return 1;
}

double rd (FILE* inpf)        // reading floating point number from a/b format
{
    int chisl = 0;
    int znam  = 1;
    char tmp;
    _Pragma ("GCC diagnostic push");
    _Pragma ("GCC diagnostic ignored \"-Wunused-result\"");
    fscanf (inpf, "%d", &chisl);
    if (fscanf (inpf, "%c", &tmp) && tmp == '/') fscanf (inpf, "%d", &znam);
    _Pragma ("GCC diagnostic pop");
    return ((double)chisl) / znam;
}

_Pragma ("GCC diagnostic push")
_Pragma ("GCC diagnostic ignored \"-Wunused-parameter\"")
static double yf (double x, double y)        // y`=yf(x,y)
{
    return -x;
}

static double xg (double y, double x)        // x`=xg(y,x)
{
    return y;
}

_Pragma ("GCC diagnostic pop")

double RK (double x0,
           double y0,
           double h,
           unsigned int s,
           double* k,
           double** cab,
           double f (double, double))        // Runge–Kutta in general form
{
    double temp;

    for (unsigned int i = 0; i < s; i++)
    {
        temp = 0;
        for (unsigned int j = 0; j < i; j++)
        {
            temp += cab[i + 1][j] * k[j];
        }
        k[i] = f (x0 + h * cab[0][i], y0 + h * temp);
    }

    temp = 0;
    for (unsigned int i = 0; i < s; i++)
    {
        temp += cab[s + 1][i] * k[i];
    }

    return y0 + h * temp;
}

void func61 (void)
{
    double h = 0.1;
    double T = 5 * pi;
    double tmp;

    double x;
    double y;
    double temp;
    double dist;

    unsigned int s;
    double* k;
    double** cab;

    /////////////////////////////////////////////////////////////////////////////////////////

    // file check

    FILE* inpf = fopen ("koef (8).txt", "r");
    if (inpf == NULL)
    {
        printf ("File doen`t exist\n");
        return;
    }
    if (!fscanf (inpf, "%ud", &s))
    {
        printf ("File isn`t correct\n");
        fclose (inpf);
        return;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix initialization

    cab = (double**)malloc ((s + 3) * sizeof (double*));
    for (unsigned int i = 0; i < s + 3; i++)
    {
        cab[i] = (double*)malloc (s * sizeof (double));
        for (unsigned int j = 0; j < s; j++)
        {
            cab[i][j] = 0;
        }
    }

    k = (double*)malloc (s * sizeof (double));
    for (unsigned int i = 0; i < s; i++)
    {
        k[i] = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix reading

    for (unsigned int i = 0; i < s; i++)
    {
        cab[0][i] = rd (inpf);
    }

    for (unsigned int i = 2; i < s + 3; i++)
        for (unsigned int j = 0; j + 1 < i && j < s && !feof (inpf); j++)
            cab[i][j] = rd (inpf);
    /*
        // CAB mmatrix printing

        printf ("\n");
        for (unsigned int i = 0; i < s + 3; i++)
        {
            for (unsigned int j = 0; j < s; j++)
                printf ("%6.3f ", cab[i][j]);
            printf ("\n");
        }
        printf ("\n");
    */
    /////////////////////////////////////////////////////////////////////////////////////////

    // Runge–Kutta for x,y

    for (int i = 1; h > 5 * pow (10., -5); i++)
    {
        printf ("Func 6.%d:\nh = %5.5f\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)\n",
                i,
                h);

        while (T < 1.5 * pow (10., 6) * pi)
        {
            x    = 0.;
            y    = 1.;
            dist = 0;
            tmp  = h;

            for (; T - dist > EPS;)
            {
                if (dist + h > T) h = T - dist;

                temp = x;
                x    = RK (y, x, h, s, k, cab, xg);
                y    = RK (temp, y, h, s, k, cab, yf);

                dist += h;
            }
            h = tmp;

            _Pragma ("GCC diagnostic push");
            _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
            if (x != x || y != y)
            {
                break;
            }
            _Pragma ("GCC diagnostic pop");

            printf (" %7.0fPi   | %11.3e  | %11.3e  \n",
                    T / pi,
                    x - sin (T),
                    y - cos (T));

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

    /////////////////////////////////////////////////////////////////////////////////////////

    fclose (inpf);
    for (unsigned int i = 0; i < s + 3; i++)
    {
        free (cab[i]);
    }
    free (cab);
    free (k);
}

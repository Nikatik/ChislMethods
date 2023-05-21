#include "lib.h"

#include <stdlib.h>

/*
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
*/
__float128
    rd (FILE* inpf)        // reading floating point number from a/b format
{
    int  chisl = 0;
    int  znam  = 1;
    char tmp;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
    fscanf (inpf, "%d", &chisl);
    if (fscanf (inpf, "%c", &tmp) && tmp == '/') fscanf (inpf, "%d", &znam);
#pragma GCC diagnostic pop
    return (__float128) (chisl) / znam;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static __float128 yfq (__float128 t, __float128 x,
                       __float128 y)        // y`=yf(x,y)
{
    return -x;
}

static __float128 xgq (__float128 t, __float128 x,
                       __float128 y)        // x`=xg(y,x)
{
    return y;
}

#pragma GCC diagnostic pop

void RK (double t, double* x0, double* y0, double h, unsigned int s, double** k,
         double** cab, double f (double, double, double),
         double g (double, double, double),
         bool   check)        // Runge–Kutta in general form
{
    double tempx;
    double tempy;

    for (unsigned int i = 0; i < s; i++)
    {
        tempx = 0;
        tempy = 0;
        for (unsigned int j = 0; j < i; j++)
        {
            tempx += cab[i + 1][j] * k[0][j];
            tempy += cab[i + 1][j] * k[1][j];
        }
        // printf("%f\n",f(1,0));
        k[0][i] = g (t + h * cab[0][i], *x0 + h * tempx, *y0 + h * tempy);
        k[1][i] = f (t + h * cab[0][i], *x0 + h * tempx, *y0 + h * tempy);
    }

    tempx = 0;
    tempy = 0;
    if (check)
    {
        for (unsigned int i = 0; i < s; i++)
        {
            tempx += cab[s + 2][i] * k[0][i];
            tempy += cab[s + 2][i] * k[1][i];
        }
    }
    else
    {
        for (unsigned int i = 0; i < s; i++)
        {
            tempx += cab[s + 1][i] * k[0][i];
            tempy += cab[s + 1][i] * k[1][i];
        }
    }

    *x0 += h * tempx;
    *y0 += h * tempy;
}

void RKq (__float128 t, __float128* x0, __float128* y0, __float128 h,
          unsigned int s, __float128** k, __float128** cab,
          __float128 g (__float128, __float128, __float128),
          __float128 f (__float128, __float128, __float128),
          bool       check)        // Runge–Kutta in general form
{
    __float128 tempx;
    __float128 tempy;

    for (unsigned int i = 0; i < s; i++)
    {
        tempx = 0;
        tempy = 0;
        for (unsigned int j = 0; j < i; j++)
        {
            tempx += cab[i + 1][j] * k[0][j];
            tempy += cab[i + 1][j] * k[1][j];
        }
        // printf("%f\n",f(1,0));
        k[0][i] = g (t + h * cab[0][i], *x0 + h * tempx, *y0 + h * tempy);
        k[1][i] = f (t + h * cab[0][i], *x0 + h * tempx, *y0 + h * tempy);
    }

    tempx = 0;
    tempy = 0;
    if (check)
    {
        for (unsigned int i = 0; i < s; i++)
        {
            tempx += cab[s + 2][i] * k[0][i];
            tempy += cab[s + 2][i] * k[1][i];
        }
    }
    else
    {
        for (unsigned int i = 0; i < s; i++)
        {
            tempx += cab[s + 1][i] * k[0][i];
            tempy += cab[s + 1][i] * k[1][i];
        }
    }

    *x0 += h * tempx;
    *y0 += h * tempy;
}

void func61 (void)
{
    __float128 h = 1;
    __float128 T = 5 * M_PIq;
    __float128 tmp;

    __float128 x;
    __float128 y;
    __float128 dist;

    unsigned int s;
    __float128** k;
    __float128** cab;

    /////////////////////////////////////////////////////////////////////////////////////////

    // file check

    FILE* inpf = fopen ("koef (8).txt", "r");
    if (inpf == NULL)
    {
        printf ("File doen`t exist\n");
        return;
    }
    if (!fscanf (inpf, "%ud", &s) || !fscanf (inpf, "%ud", &s))
    {
        printf ("File isn`t correct\n");
        fclose (inpf);
        return;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix initialization

    cab = (__float128**) malloc ((s + 3) * sizeof (__float128*));
    for (unsigned int i = 0; i < s + 3; i++)
    {
        cab[i] = (__float128*) malloc (s * sizeof (__float128));
        for (unsigned int j = 0; j < s; j++) { cab[i][j] = 0; }
    }

    k    = (__float128**) malloc (2 * sizeof (__float128*));
    k[0] = (__float128*) malloc (s * sizeof (__float128));
    k[1] = (__float128*) malloc (s * sizeof (__float128));
    for (unsigned int i = 0; i < s; i++)
    {
        k[0][i] = 0;
        k[1][i] = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix reading

    for (unsigned int i = 0; i < s; i++) { cab[0][i] = rd (inpf); }

    for (unsigned int i = 2; i < s + 3; i++)
        for (unsigned int j = 0; j + 1 < i && j < s && !feof (inpf); j++)
            cab[i][j] = rd (inpf);

    // CAB matrix printing
    /*
    printf ("\n");
    for (unsigned int i = 0; i < s + 3; i++)
    {
        for (unsigned int j = 0; j < s; j++) printf ("%6.3f ", cab[i][j]);
        printf ("\n");
    }
    printf ("\n");
    // */
    /////////////////////////////////////////////////////////////////////////////////////////

    // Runge–Kutta for harmonic oscillator

    for (int i = 1; h > 5 * (__float128) pow (10, -3); i++)
    {
        printf ("Func 6.%d:\nh = %5.5Lf\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)\n",
                i, (long double) h);

        while (T < 1.5Q * (__float128) pow (10, 4) * M_PIq)
        {
            dist = 0;
            x    = sinq (dist);
            y    = cosq (dist);
            tmp  = h;

            for (; T - dist > EPS;)
            {
                if (dist + h > T) h = T - dist;

                RKq (dist, &x, &y, h, s, k, cab, xgq, yfq, false);
                dist += h;
                // printf("%.17e\n",y);
            }
            h = tmp;


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
            if (x != x || y != y) { break; }
#pragma GCC diagnostic pop

            printf (" %7.0LfPi   | %11.3Le  | %11.3Le  \n",
                    (long double) (T / M_PIq), (long double) (x - sinq (T)),
                    (long double) (y - cosq (T)));

            if (T < 9 * M_PIq)
            {
                T += 5 * M_PIq;
                continue;
            }
            T *= 10;
        }

        T = 5 * M_PIq;
        printf ("\n");
        h *= 0.1Q;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // cleaning :)

    fclose (inpf);
    for (unsigned int i = 0; i < s + 3; i++) { free (cab[i]); }
    free (cab);
    free (k[0]);
    free (k[1]);
    free (k);
}

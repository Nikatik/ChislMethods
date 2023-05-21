#include "lib.h"

#include <stdlib.h>
#include <time.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static __float128 myf (__float128 t, __float128 x,
                       __float128 y)        // y`=yf(x,y)
{
    return -2.Q * t * y * logq (fmaxq (x, (__float128) pow (10, -3)));
}

static __float128 mxg (__float128 t, __float128 x,
                       __float128 y)        // x`=xg(y,x)
{
    return 2.Q * t * x * logq (fmaxq (y, (__float128) pow (10, -3)));
}

static __float128 yf (__float128 t, __float128 x,
                      __float128 y)        // y`=yf(x,y)
{
    return -x;
}

static __float128 xg (__float128 t, __float128 x,
                      __float128 y)        // x`=xg(y,x)
{
    return y;
}

#pragma GCC diagnostic pop

__float128 astep (__float128 T, __float128* x, __float128* y, __float128* x_,
                  __float128* y_, long long unsigned* i, long long unsigned* j,
                  unsigned int p, unsigned int s, __float128** k,
                  __float128** cab, __float128 tol,
                  __float128 f (__float128, __float128, __float128),
                  __float128 g (__float128, __float128, __float128))
{
    __float128 tempx, tempy, dist, h, temp, fac, err;
    *x_ = tempx = *x;
    *y_ = tempy = *y;
    dist        = 0;
    h           = 0.01Q;
    fac         = 1.7Q;
    err         = 0;
    for (*i = *j = 0; T - dist > EPS;)
    {
        if (dist + h > T) h = T - dist;

        RKq (dist, &tempx, &tempy, h, s, k, cab, f, g, false);
        RKq (dist, x_, y_, h, s, k, cab, f, g, true);

        temp = h;
        h *= fminq (fac, fmaxq (0.7Q, powq (0.98Q * tol /
                                                fmaxq (fabsq (tempx - *x_),
                                                       fabsq (tempy - *y_)),
                                            1.Q / (p + 1))));

        if (h < (__float128) pow (10, -18))
        {
            printf ("\nSomething goes wrong...\n");
            return 0;
        }
        if (fmaxq (fabsq (tempx - *x_), fabsq (tempy - *y_)) > tol)
        {
            *x_ = tempx = *x;
            *y_ = tempy = *y;
            fac         = 1;
            *j += 1;
            continue;
        }

        err += fmaxq (fabsq (tempx - *x_), fabsq (tempy - *y_));
        dist += temp;
        *x  = tempx;
        *y  = tempy;
        fac = 1.7Q;
        *i += 1;
    }
    return err;
}

void func71 (void)
{
    __float128         T   = 5 * M_PIq;
    __float128         tol = (__float128) pow (10, -7);
    long long unsigned i   = 0;
    long long unsigned j   = 0;

    __float128 x;
    __float128 y;
    __float128 x_;
    __float128 y_;
    __float128 err;

    unsigned int s;
    unsigned int p;
    __float128** k;
    __float128** cab;

    time_t start, end;

    /////////////////////////////////////////////////////////////////////////////////////////

    // file check

    time (&start);

    FILE* inpf = fopen ("koef (8).txt", "r");
    if (inpf == NULL)
    {
        printf ("File doen`t exist\n");
        return;
    }
    if (!fscanf (inpf, "%ud", &p) || !fscanf (inpf, "%ud", &s))
    {
        printf ("File isn`t correct\n");
        fclose (inpf);
        return;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix initialization

    cab = (__float128**) malloc ((s + 3) * sizeof (__float128*));
    for (i = 0; i < s + 3; i++)
    {
        cab[i] = (__float128*) malloc (s * sizeof (__float128));
        for (j = 0; j < s; j++) { cab[i][j] = 0; }
    }

    k    = (__float128**) malloc (2 * sizeof (__float128*));
    k[0] = (__float128*) malloc (s * sizeof (__float128));
    k[1] = (__float128*) malloc (s * sizeof (__float128));
    for (i = 0; i < s; i++)
    {
        k[0][i] = 0;
        k[1][i] = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix reading

    for (i = 0; i < s; i++) { cab[0][i] = rd (inpf); }

    for (i = 2; i < s + 3; i++)
        for (j = 0; j + 1 < i && j < s && !feof (inpf); j++)
            cab[i][j] = rd (inpf);

    // CAB matrix printing
    /*
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

    // Runge-Kutta for harmonic oscillator

    printf ("Func 7.1: \n");

    T = 10 * M_PIq;
    printf ("Tolerance = %.0Le\n      T      |  x*(T)-x(T)  |  "
            "z*(T)-z(T)  |      i      |      j\n",
            (long double) pow (10, -20));
    x = 0;
    y = 1;
    err = astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab,
                 (__float128) pow (10, -20), xg, yf);

    printf (" %7.0LfPi   | %11.3Le  | %11.3Le  | %10llu  | %10llu  | "
            "%11.3Le  | %11.3Le  | %11.3Le  \n",
            (long double) (T / M_PIq), (long double) (x - x_),
            (long double) (y - y_), i, j, (long double) err,
            (long double) (x - sinq (T)), (long double) (y - cosq (T)));
    printf ("\n");

    for (tol = (__float128) pow (10, -7); tol > 5 * (__float128) pow (10, -12);
         tol *= 0.01Q)
    {
        T = 5 * M_PIq;
        printf ("Tolerance = %.0Le\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)  |      i      |      j\n",
                (long double) tol);
        for (; T < 1.5Q * (__float128) pow (10, 4) * M_PIq;)
        {
            x = 0;
            y = 1;

            err =
                astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab, tol, xg, yf);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
            if (x != x || y != y || x_ != x_ || y_ != y_) { break; }
#pragma GCC diagnostic pop

            printf (" %7.0LfPi   | %11.3Le  | %11.3Le  | %10llu  | %10llu  | "
                    "%11.3Le  | %11.3Le  | %11.3Le  \n",
                    (long double) (T / M_PIq), (long double) (x - x_),
                    (long double) (y - y_), i, j, (long double) err,
                    (long double) (x - sinq (T)), (long double) (y - cosq (T)));
            if (T < 9 * M_PIq)
            {
                T += 5 * M_PIq;
                continue;
            }
            T *= 10;
        }
        printf ("\n");
    }

    __float128 tempx7 = 0, tempy7 = 1, tempx9 = 0, tempy9 = 1;
    astep ((__float128) pow (10, 4) * M_PIq, &tempx7, &tempy7, &x_, &y_, &i, &j,
           p, s, k, cab, (__float128) pow (10, -7), xg, yf);
    astep ((__float128) pow (10, 4) * M_PIq, &tempx9, &tempy9, &x_, &y_, &i, &j,
           p, s, k, cab, (__float128) pow (10, -9), xg, yf);
    printf ("Rx(%.1e*Pi) = %.0Lf\tRy(%.1e*Pi) = %.0Lf\n", pow (10, 4),
            (long double) fabsq ((tempx7 - tempx9) / (tempx9 - x)), pow (10, 4),
            (long double) fabsq ((tempy7 - tempy9) / (tempy9 - y)));
    for (T = 75 * (__float128) pow (10, 2) * M_PIq;
         T > 10 * (__float128) pow (10, 2) * M_PIq;
         T -= 25 * (__float128) pow (10, 2) * M_PIq)
    {
        x = tempx7 = tempx9 = 0;
        y = tempy7 = tempy9 = 1;
        astep (T, &tempx7, &tempy7, &x_, &y_, &i, &j, p, s, k, cab,
               (__float128) pow (10, -7), xg, yf);
        astep (T, &tempx9, &tempy9, &x_, &y_, &i, &j, p, s, k, cab,
               (__float128) pow (10, -9), xg, yf);
        astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab,
               (__float128) pow (10, -11), xg, yf);
        printf ("Rx(%.1Lf*Pi) = %.0Lf\tRy(%.1Lf*Pi) = %.0Lf\n",
                (long double) (T / M_PIq),
                (long double) fabsq ((tempx7 - tempx9) / (tempx9 - x)),
                (long double) (T / M_PIq),
                (long double) fabsq ((tempy7 - tempy9) / (tempy9 - y)));
    }
    printf ("\n\n");

    /////////////////////////////////////////////////////////////////////////////////////////

    // Runge-Kutta for brusselator ... OH, NO, IT`S SOMETHING DIFFERENT!!!
    // WTF!?1?

    printf ("Func 7.2: \n");

    T = 10 * M_PIq;
    printf ("Tolerance = %.0Le\n      T      |  x*(T)-x(T)  |  "
            "z*(T)-z(T)  |      i      |      j\n",
            (long double) pow (10, -20));
    x = 1;
    y = expq (1);
    err = astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab,
                 (__float128) pow (10, -20), mxg, myf);

    printf (" %7.0LfPi   | %11.3Le  | %11.3Le  | %10llu  | %10llu  | "
            "%11.3Le    | %11.3Le  | %11.3Le  \n",
            (long double) (T / M_PIq), (long double) (x - x_),
            (long double) (y - y_), i, j, (long double) err,
            (long double) (x - expq(sinq (powq(T, 2)))), (long double) (y - expq(cosq (powq(T, 2)))));
    printf ("\n");

    for (tol = (__float128) pow (10, -7); tol > 5 * (__float128) pow (10, -12);
         tol *= 0.01Q)
    {
        T = 5 * M_PIq;
        printf ("Tolerance = %.0Le\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)  |      i      |      j\n",
                (long double) tol);
        for (; T < 1.5Q * (__float128) pow (10, 2) * M_PIq;)
        {
            x = 1;
            y = expq (1);

            err = astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab, tol, mxg,
                         myf);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
            if (x != x || y != y || x_ != x_ || y_ != y_) { break; }
#pragma GCC diagnostic pop

            printf (" %7.0LfPi   | %11.3Le  | %11.3Le  | %10llu  | %10llu  | "
                    "%11.3Le  \n| %11.3Le  | %11.3Le  \n",
                    (long double) (T / M_PIq), (long double) (x - x_),
                    (long double) (y - y_), i, j, (long double) err,
                    (long double) (x - expq(sinq (powq(T, 2)))), (long double) (y - expq(cosq (powq(T, 2)))));

            if (T < 9 * M_PIq)
            {
                T += 5 * M_PIq;
                continue;
            }
            T *= 10;
        }
        printf ("\n");
    }

    tempx7 = tempx9 = 1;
    tempy7 = tempy9 = expq (1);
    astep ((__float128) pow (10, 2) * M_PIq, &tempx7, &tempy7, &x_, &y_, &i, &j,
           p, s, k, cab, (__float128) pow (10, -7), mxg, myf);
    astep ((__float128) pow (10, 2) * M_PIq, &tempx9, &tempy9, &x_, &y_, &i, &j,
           p, s, k, cab, (__float128) pow (10, -9), mxg, myf);
    printf ("Rx(%.1e*Pi) = %.0Lf\tRy(%.1e*Pi) = %.0Lf\n", pow (10, 2),
            (long double) fabsq ((tempx7 - tempx9) / (tempx9 - x)), pow (10, 2),
            (long double) fabsq ((tempy7 - tempy9) / (tempy9 - y)));
    for (T = 75 * M_PIq; T > 10 * M_PIq; T -= 25 * M_PIq)
    {
        x = tempx7 = tempx9 = 1;
        y = tempy7 = tempy9 = expq (1);
        astep (T, &tempx7, &tempy7, &x_, &y_, &i, &j, p, s, k, cab,
               (__float128) pow (10, -7), mxg, myf);
        astep (T, &tempx9, &tempy9, &x_, &y_, &i, &j, p, s, k, cab,
               (__float128) pow (10, -9), mxg, myf);
        astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab,
               (__float128) pow (10, -11), mxg, myf);
        printf ("Rx(%.1Lf*Pi) = %.0Lf\tRy(%.1Lf*Pi) = %.0Lf\n",
                (long double) (T / M_PIq),
                (long double) fabsq ((tempx7 - tempx9) / (tempx9 - x)),
                (long double) (T / M_PIq),
                (long double) fabsq ((tempy7 - tempy9) / (tempy9 - y)));
    }
    printf ("\n\n");

    /////////////////////////////////////////////////////////////////////////////////////////

    // cleaning :)

    fclose (inpf);
    for (i = 0; i < s + 3; i++) { free (cab[i]); }
    free (cab);
    free (k[0]);
    free (k[1]);
    free (k);

    time (&end);
    // printf ("%.f seconds\n", difftime (end, start));
}

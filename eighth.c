#include "lib.h"

#include <quadmath.h>
#include <stdlib.h>
#include <time.h>
#define EPSM (__float128) pow (10, -25)
#define DIST 0

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static __float128 yf (__float128 t, __float128 x,
                      __float128 y)        // y`=yf(x,y)
{
     return x;
    // return cosq (t) - (1 + 0.001Q * powq (x, 2)) * x;
}

static __float128 xg (__float128 t, __float128 x,
                      __float128 y)        // x`=xg(y,x)
{
     return -y;
    // return y;
}

#pragma GCC diagnostic pop

void printing (__float128 dist, __float128 x, __float128 y)
{
    __float128 fractpart, intpart;

    printf ("Func 8.1: \n        t       | %6c       | %6c       \n", 'x', 'y');
    fractpart = modfq (dist, &intpart);
    printf ("%6.0Lf + %.3Lf  | %11.3Le  | %11.3Le  \n\n", (long double) intpart,
            (long double) fractpart, (long double) (x - cosq (dist)),
            (long double) (y - sinq (dist)));
}

bool astepCH (__float128* startx, __float128* starty, __float128* dist,
              __float128 h, unsigned int p, unsigned int s, __float128** k,
              __float128** cab, __float128 tol,
              __float128 f (__float128, __float128, __float128),
              __float128 g (__float128, __float128, __float128))
{
    __float128 tempx;           //  next step x
    __float128 tempy;           //  next step y
    __float128 x;               //  current step x
    __float128 y;               //  current step y
    __float128 x_;              //  next check step x
    __float128 y_;              //  next check step y
    __float128 leftx;           //  startx step x
    __float128 leftdist;        //  startx step distance
    __float128 temp;            //  theoretical step
    __float128 fac;             //  multiplier for step
    bool       first;
    bool       cross = false;
    bool       start = true;

    leftx = x_ = tempx = x = *startx;
    y_ = tempy = y = *starty;
    fac            = 1.7Q;
    leftdist       = *dist;

    // printf (" %11c  | %9c  | %11c  | %11c  \n", 'h', 't', 'x', 'y');

    for (;;)
    {
        first = false;

        RKq (*dist, &tempx, &tempy, h, s, k, cab, f, g, false);
        RKq (*dist, &x_, &y_, h, s, k, cab, f, g, true);

        temp = h;
        h *= fminq (fac, fmaxq (0.7Q, powq (0.98Q * tol /
                                                fmaxq (fabsq (tempx - x_),
                                                       fabsq (tempy - y_)),
                                            1.Q / (p + 1))));

        if (fabsq (h) < (__float128) pow (10, -10) ||
            fabsq (*dist) >
                3 * (__float128) pow (10, 2))        // too small step, aborting
        {
            *startx = x;
            *starty = y;
            return false;
        }
        if (fmaxq (fabsq (tempx - x_), fabsq (tempy - y_)) >
            tol)        // too big err, droping step
        {
            x_ = tempx = x;
            y_ = tempy = y;
            fac        = 1;
            continue;
        }

        // check
        if (fabsq (y - tempy) /
                fmaxq (fabsq (tempy - *starty), fabsq (y - *starty)) >
            1.02Q)        // next step crossed starty
        {
            if (cross)        // cross after full rotation
            {
                __float128 lx = x, ly = y, ldist = *dist, _x = x, _y = y,
                           _h = temp, _dist = *dist;

                for (; fabsq (_y - *starty) >
                       EPSM;)        // finding point of crossing
                {
                    if ((_y - *starty) * (tempy - *starty) < 0)
                    {
                        lx    = _x;
                        ly    = _y;
                        ldist = _dist;
                        _h    = fabsq (_y - *starty) / fabsq (tempy - _y) *
                             (*dist + temp -
                              _dist);        // step from _y to starty

                        RKq (_dist, &_x, &_y, _h, s, k, cab, f, g, false);
                        _dist += _h;
                    }
                    else
                    {
                        tempx = _x;
                        tempy = _y;
                        temp  = _dist - *dist;

                        _h = fabsq (ly - *starty) / fabsq (ly - _y) *
                             (_dist - ldist);        // step from *y to starty

                        _x    = lx;
                        _y    = ly;
                        _dist = ldist;

                        RKq (_dist, &_x, &_y, _h, s, k, cab, f, g, false);

                        _dist += _h;
                    }
                }

                tempx = x;
                tempy = y;
                temp  = _dist - *dist;

                RKq (*dist, &tempx, &tempy, temp, s, k, cab, f, g,
                     false);        // counting starty point

                if (fabsq (leftx - tempx) > EPSM)
                {
                    leftx    = tempx;
                    leftdist = *dist;
                    first    = true;
                }
                cross = false;
            }
            else { cross = true; }

            // printf (" %11.3e  | %9.3f  | %11.3e  | %11.3e  \t| %11.3e  |
            // %11.3e  \n", h, *dist/pi, tempx, tempy, *startx, leftx);

            if (start)
            {
                leftx    = tempx;
                leftdist = *dist;
                first    = true;
                start    = false;
            }
        }

        *dist += temp;
        if (fabsq (tempx - leftx) < EPSM &&
            fabsq (*dist - leftdist) > 5 * fabsq (h) &&
            !first)        // closed loop
        // if(*dist > 1.3 * pow(10,2))
        {
            x = tempx;
            y = tempy;
            break;
        }
        x   = tempx;
        y   = tempy;
        fac = 1.7Q;

        // printf ("%.17E\n", x);
    }
    *startx = x;
    *starty = y;
    return true;
}

void func81 (void)
{
    __float128         dist;
    __float128         tol = EPSM * (__float128) pow (10, -5);
    long long unsigned i   = 0;
    long long unsigned j   = 0;

    __float128 x;
    __float128 y;

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

        /*
    dist = 0;
    x    = 1;
    y    = 0;
    //  */
    //  /*
    dist = DIST;
    x    = cosq (dist);
    y    = sinq (dist);
    //  */
    if (!astepCH (&x, &y, &dist, 0.01Q, p, s, k, cab, tol, xg, yf))
    {
            /*
        dist = 0;
        x    = 1;
        y    = 0;
        //  */
        //  /*
        dist = DIST;
        x    = cosq (dist);
        y    = sinq (dist);
        //  */
        if (!astepCH (&x, &y, &dist, -0.01Q, p, s, k, cab, tol, xg, yf))
            printf ("\nSomething goes wrong...\n");
        else printing (dist, x, y);
    }
    else printing (dist, x, y);

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

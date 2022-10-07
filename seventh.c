#include "lib.h"
#include <stdlib.h>
#include <time.h>

_Pragma ("GCC diagnostic push")
_Pragma ("GCC diagnostic ignored \"-Wunused-parameter\"") 
static double myf (double t, double x, double y)        // y`=yf(x,y)
{
    return -2*t*y*log(fmax(x,pow(10,-3)));
}

static double mxg (double t, double x, double y)        // x`=xg(y,x)
{
    return 2*t*x*log(fmax(y,pow(10,-3)));
}

static double yf (double t, double x, double y)        // y`=yf(x,y)
{
    return -x;
}

static double xg (double t, double x, double y)        // x`=xg(y,x)
{
    return y;
}

_Pragma ("GCC diagnostic pop")

void astep (double T,
            double* x,
            double* y,
            double* x_,
            double* y_,
            long long unsigned* i,
            long long unsigned* j,
            unsigned int p,
            unsigned int s,
            double** k,
            double** cab,
            double tol,
            double f (double, double, double),
            double g (double, double, double))
{
    double tempx, tempy, dist, h, temp, fac;
    *x_ = tempx = *x;
    *y_ = tempy = *y;
    dist        = 0;
    h           = 0.01;
    fac         = 1.7;
    for (*i = *j = 0; T - dist > EPS;)
    {
        if (dist + h > T) h = T - dist;

        RK (dist, &tempx, &tempy, h, s, k, cab, f, g, false);
        RK (dist, x_, y_, h, s, k, cab, f, g, true);

        temp = h;
        h *= fmin (fac,
                   fmax (0.7,
                         pow (0.98 * tol /
                                  fmax (fabs (tempx - *x_), fabs (tempy - *y_)),
                              1. / (p + 1))));

        if (h<pow(10,-18))
        {
            printf("\nSomething goes wrong...\n");
            return;
        }
        if (fmax (fabs (tempx - *x_), fabs (tempy - *y_)) > tol)
        {
            *x_ = tempx = *x;
            *y_ = tempy = *y;
            fac         = 1;
            *j += 1;
            continue;
        }

        dist += temp;
        *x  = tempx;
        *y  = tempy;
        fac = 1.7;
        *i += 1;
    }
}

void func71 (void)
{
    double T             = 5. * pi;
    double tol           = pow (10, -7);
    long long unsigned i = 0;
    long long unsigned j = 0;

    double x;
    double y;
    double x_;
    double y_;

    unsigned int s;
    unsigned int p;
    double** k;
    double** cab;

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

    cab = (double**)malloc ((s + 3) * sizeof (double*));
    for (i = 0; i < s + 3; i++)
    {
        cab[i] = (double*)malloc (s * sizeof (double));
        for (j = 0; j < s; j++)
        {
            cab[i][j] = 0;
        }
    }

    k    = (double**)malloc (2 * sizeof (double*));
    k[0] = (double*)malloc (s * sizeof (double));
    k[1] = (double*)malloc (s * sizeof (double));
    for (i = 0; i < s; i++)
    {
        k[0][i] = 0;
        k[1][i] = 0;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

    // CAB matrix reading

    for (i = 0; i < s; i++)
    {
        cab[0][i] = rd (inpf);
    }

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
    for (; tol > 5 * pow (10, -12); tol *= 0.01)
    {
        T = 5. * pi;
        printf ("Tolerance = %.0e\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)  |      i      |      j\n",
                tol);
        for (; T < 1.5 * pow (10, 6) * pi;)
        {
            x = 0;
            y = 1;

            astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab, tol, yf, xg);

            _Pragma ("GCC diagnostic push");
            _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
            if (x != x || y != y || x_ != x_ || y_ != y_)
            {
                break;
            }
            _Pragma ("GCC diagnostic pop");

            printf (" %7.0fPi   | %11.3e  | %11.3e  | %10llu  | %10llu  \n",
                    T / pi,
                    x - x_,
                    y - y_,
                    i,
                    j);
            if (T < 9 * pi)
            {
                T += 5 * pi;
                continue;
            }
            T *= 10;
        }
        printf ("\n");
    }
    
    double tempx7 = 0, tempy7 = 1, tempx9 = 0, tempy9 = 1;
    astep (pow(10,6)*pi, &tempx7, &tempy7, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-7), yf, xg);
    astep (pow(10,6)*pi, &tempx9, &tempy9, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-9), yf, xg);
    printf("Rx(%.1e*Pi) = %.0f\tRy(%.1e*Pi) = %.0f\n",pow(10,6),fabs((tempx7-tempx9)/(tempx9-x)),pow(10,6),fabs((tempy7-tempy9)/(tempy9-y)));
    for(T=75*pow(10,4)*pi;T>10*pow(10,4)*pi;T-=25*pow(10,4)*pi)
    {
        x=tempx7=tempx9=0;
        y=tempy7=tempy9=1;
        astep (T, &tempx7, &tempy7, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-7), yf, xg);
        astep (T, &tempx9, &tempy9, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-9), yf, xg);
        astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-11), yf, xg);
        printf("Rx(%.1e*Pi) = %.0f\tRy(%.1e*Pi) = %.0f\n",T/pi,fabs((tempx7-tempx9)/(tempx9-x)),T/pi,fabs((tempy7-tempy9)/(tempy9-y)));

    }
    printf("\n\n");

    /////////////////////////////////////////////////////////////////////////////////////////

    // Runge-Kutta for brusselator ... OH, NO, IT`S SOMETHING DIFFERENT!!! WTF!?1?

    printf ("Func 7.2: \n");
    for (tol = pow (10, -7); tol > 5 * pow (10, -12); tol *= 0.01)
    {
        T = 5.*pi;
        printf ("Tolerance = %.0e\n      T      |  x*(T)-x(T)  |  "
                "z*(T)-z(T)  |      i       |      j\n",
                tol);
        for (; T < 1.5 * pow (10, 3) * pi;)
        {
            x = 1;
            y = exp(1);

            astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab, tol, myf, mxg);

            _Pragma ("GCC diagnostic push");
            _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
            if (x != x || y != y || x_ != x_ || y_ != y_)
            {
                break;
            }
            _Pragma ("GCC diagnostic pop");

            printf (" %7.0fPi   | %11.3e  | %11.3e  | %10llu   | %10llu  \n",
                    T/pi,
                    x - x_,
                    y - y_,
                    i,
                    j);

            if (T < 9.*pi)
            {
                T += 5.*pi;
                continue;
            }
            T *= 10.;
        }
        printf ("\n");
    }
    
    tempx7 = tempx9 = 1;
    tempy7 = tempy9 = exp(1);
    astep (pow(10,3)*pi, &tempx7, &tempy7, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-7), myf, mxg);
    astep (pow(10,3)*pi, &tempx9, &tempy9, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-9), myf, mxg);
    printf("Rx(%.1e*Pi) = %.0f\tRy(%.1e*Pi) = %.0f\n",pow(10,3),fabs((tempx7-tempx9)/(tempx9-x)),pow(10,3),fabs((tempy7-tempy9)/(tempy9-y)));
    for(T=750*pi;T>100*pi;T-=250*pi)
    {
        x=tempx7=tempx9=1;
        y=tempy7=tempy9=exp(1);
        astep (T, &tempx7, &tempy7, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-7), myf, mxg);
        astep (T, &tempx9, &tempy9, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-9), myf, mxg);
        astep (T, &x, &y, &x_, &y_, &i, &j, p, s, k, cab, pow(10,-11), myf, mxg);
        printf("Rx(%.1e*Pi) = %.0f\tRy(%.1e*Pi) = %.0f\n",T/pi,fabs((tempx7-tempx9)/(tempx9-x)),T/pi,fabs((tempy7-tempy9)/(tempy9-y)));

    }
    printf("\n\n");
    
    /////////////////////////////////////////////////////////////////////////////////////////

    // cleaning :)
    
    fclose (inpf);
    for (i = 0; i < s + 3; i++)
    {
        free (cab[i]);
    }
    free (cab);
    free (k[0]);
    free (k[1]);
    free (k);
    
    time (&end);
    printf ("%.f seconds\n", difftime (end, start));
}

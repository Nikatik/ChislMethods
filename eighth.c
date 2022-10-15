#include "lib.h"
#include <stdlib.h>
#include <time.h>
#define EPSM pow(10, -7)
_Pragma ("GCC diagnostic push")
_Pragma ("GCC diagnostic ignored \"-Wunused-parameter\"") 

static double yf (double t, double x, double y)        // y`=yf(x,y)
{
    //return x;
    return cos(t)-(1+0.2*pow(x,2))*x;
}

static double xg (double t, double x, double y)        // x`=xg(y,x)
{
    //return -y;
    return y;
}

_Pragma ("GCC diagnostic pop")

void astepCH (double* startx,
            double* starty,
            double* dist,
            unsigned int p,
            unsigned int s,
            double** k,
            double** cab,
            double tol,
            double f (double, double, double),
            double g (double, double, double))
{
    double tempx;       //  next step x
    double tempy;       //  next step y
    double x;           //  current step x
    double y;           //  current step y
    double x_;          //  next check step x
    double y_;          //  next check step y
    double leftx;       //  startx step x
    double h;           //  real step 
    double temp;        //  theoretical step
    double fac;         //  multiplier for step
    bool first;
    bool start = true;
    
    leftx = x_ = tempx = x = *startx;
    y_ = tempy = y = *starty;
    h           = 0.01;
    fac         = 1.7;
    
    for (;;)
    {
        first = false;

        RK (*dist, &tempx, &tempy, h, s, k, cab, f, g, false);
        RK (*dist, &x_, &y_, h, s, k, cab, f, g, true);
        
        
        temp = h;
        h *= fmin (fac,
                   fmax (0.7,
                         pow (0.98 * tol /
                                  fmax (fabs (tempx - x_), fabs (tempy - y_)),
                              1. / (p + 1))));

        if (h < pow(10, -18))                                                                   // too small step, aborting
        {
            printf("\nSomething goes wrong...\n");
            return;
        }
        if (fmax (fabs (tempx - x_), fabs (tempy - y_)) > tol)                                  // too big err, droping step
        {
            x_ = tempx = x;
            y_ = tempy = y;
            fac         = 1;
            continue;
        }
        
        //check
        if((y - *starty) * (tempy - *starty) < 0)                                               // next step crossed starty
        {
            if((leftx - *startx) * (tempx - *startx) > 0)                                       // cross after full rotation
            {
                //printf("|\n");
                double _y = y, _x = x, _h = temp, _dist = *dist;
                for(;fabs(_y - *starty) > EPSM;)                                                 // finding point of crossing
                {
                    if((_y - *starty) * (tempy - *starty) < 0){
                        
                        _h = fabs(_y - *starty) / fabs(tempy - _y) * (*dist + temp - _dist);    // step from _y to starty
                        
                        RK (_dist, &_x, &_y, _h, s, k, cab, f, g, false);
                        _dist+=_h;

                    }else{
                        
                        _h = fabs(y - *starty) / fabs(y - _y) * (_dist - *dist);                 // step from *y to starty
                        
                        _x=x;
                        _y=y;
                        _dist=*dist;
                        
                        RK (_dist, &_x, &_y, _h, s, k, cab, f, g, false);
                        
                        _dist+=_h;
                        
                    }
                }
                
                tempx = x;
                tempy = y;
                temp = _dist - *dist;
                RK (*dist, &tempx, &tempy, temp, s, k, cab, f, g, false);                        // counting starty point
                if(fabs(leftx - tempx) > EPSM)
                {
                    leftx = tempx;
                    first = true;
                }
            }
            
            if(start)
            {
                leftx = tempx;
                first = true;
                start = false;
            }
        }
        
        *dist += temp;
        if(fabs(tempx - leftx) < EPSM && !first)                                                           // closed loop
        //if(*dist > 1.3 * pow(10,2))
        { 
            x  = tempx;
            y  = tempy;
            break;
        }
        x  = tempx;
        y  = tempy;
        fac = 1.7;
        
        //printf ("%.17E\n", x);
    }
    *startx = x;
    *starty = y;
}

void func81 (void)
{
    double dist = 0;
    double tol           = EPSM * pow(10, -7);
    long long unsigned i = 0;
    long long unsigned j = 0;

    double x;
    double y;

    unsigned int s;
    unsigned int p;
    double** k;
    double** cab;

    time_t start, end;
    
    double fractpart, intpart;
    
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

    printf ("Func 8.1: \n");
    
    x = 1;
    y = 0;
/*
    x=cos(dist);
    y=sin(dist);
*/    
    astepCH (&x, &y, &dist, p, s, k, cab, tol, yf, xg);

    fractpart = modf (dist, &intpart);
//    /*
    printf ("%.0f + %.3f  | %11.3e  | %11.3e  \n",
            intpart,
            fractpart,
            x,
            y);
    
    printf ("\n");
//    */   
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
    //printf ("%.f seconds\n", difftime (end, start));
}

#include "lib.h"
#define EPS pow(10,-15)
#define pi 3.14159265359

double yf(double x, double y)
{
    return -x;
}

double xg(double x, double y)
{
    return y;
}

double RK(double x0, double y0, double h, double T)
{
    double x1=x0;
    double tempy;
    double res;
    double k[13];
    double cab[117];

    for(int i=0;i<13;i++)
    {
        k[i]=0;
    }
    for(int i=0;i<117;i++)
    {
        cab[i]=0;
    }
    
    

    return res;
}

void func61 (void)
{
    double h = 0.1;
    double T = 5 * pi;
}

#include "lib.h"
#define pi 3.14159265359

void func51 (void)
{
    double h=0.1;
    double T=5*pi;

    double x;
    double z;
    double temp;

    for (int i=1;h > pow (10., -4);i++)
    {
	printf ("Func 5.%d:\nh = %5.4f\n      T      |  x*(T)-x(T)  |  z*(T)-z(T)\n", i, h);
        while (T<pow(10.,6))
        {
            x=0.;
            z=1.;
            for(unsigned long long int j=0; (double) j * h < T; j++)
            {
                temp=x;
                x+=h*z;
                z-=h*temp;
            }
            _Pragma ("GCC diagnostic push");
            _Pragma ("GCC diagnostic ignored \"-Wfloat-equal\"");
            if(x!=x||z!=z) { break;}
            _Pragma ("GCC diagnostic pop");
            printf(" %7.0fPi   | %11.3e  | %11.3e  \n", (int) T/pi, x-sin(T), z-cos(T));
            if (T<9.*pi)
            {
                T+=5*pi;
                continue;
            }
            if (T>95000.*pi)
            {
                T+=100000.*pi;
                continue;
            }
            if (h>0.002&&T>9500.*pi)
            {
                T+=10000.*pi;
                continue;
            }
            if (h>0.02&&T>950.*pi)
            {
                T+=1000.*pi;
                continue;
            }
            T*=10.;
        }
        T=5*pi;
        printf ("\n");
        h *= 0.1;
    }
    printf ("\n");
}

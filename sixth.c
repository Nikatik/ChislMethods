#include "lib.h"
#include <stdlib.h>

#define EPS pow (10, -15)
#define pi 3.14159265359

int rs (FILE* inpf)
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
            fscanf (inpf, "%c", temp + i);
            if (temp[i] == ' ' || temp[i] == '\n' || temp[i] == '\0')
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

double rd (FILE* inpf)
{
    int chisl = 0;
    int znam  = 1;
    char tmp;
    fscanf (inpf, "%d", &chisl);
    if (fscanf (inpf, "%c", &tmp) && tmp == '/') fscanf (inpf, "%d", &znam);
    return ((double)chisl) / znam;
}

double yf (double x, double y)
{
    return -x;
}

double xg (double x, double y)
{
    return y;
}

/*
double RK (double x0, double y0, double h, double T, int s, double** cab)
{
    double x1    = x0;
    double tempy = y0;
    double res   = tempy;
    double* k;

    return res;
}
*/
void func61 (void)
{
    double h = 0.1;
    double T = 5 * pi;
    unsigned int s;
    double** cab;

    FILE* inpf = fopen ("koef.txt", "r");
    if (inpf == NULL)
    {
        printf ("File doen`t exist\n");
        return;
    }
    if (!fscanf (inpf, "%ud", &s))
    {
        printf ("File isn`t correct\n");
        return;
    }

    cab = (double**)malloc ((s + 3) * sizeof (double*));
    for (unsigned int i = 0; i < s + 3; i++)
    {
        cab[i] = (double*)malloc (s * sizeof (double));
        for (unsigned int j = 0; j < s; j++)
        {
            cab[i][j] = 0;
        }
    }

    for (unsigned int i = 0; i < s; i++)
    {
        /*if (!rs (inpf))
        {
            printf ("File isn`t correct\n");
            return;
        }*/
        cab[0][i] = rd (inpf);
    }

    for (unsigned int i = 2; i < s + 2; i++)
        for (unsigned int j = 0; j + 1 < i && j < s && !feof (inpf); j++)
            cab[i][j] = rd (inpf);

    for (unsigned int i = 0; i < s + 3; i++)
    {
        for (unsigned int j = 0; j < s; j++)
            printf ("%13.5f ", cab[i][j]);
        printf ("\n");
    }

    fclose (inpf);
    for (int i = 0; i < s + 3; i++)
        free (cab[i]);
    free (cab);
}

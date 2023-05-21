#include "lib.h"

#include <time.h>

void all (void)
{
    printf ("Func 1.a:%13.4Le\nFunc 1.b:%13.4Le\nFunc 1.c:%13.4Le\n",
            ldexpl ((long double)1., -func1()), (long double)func2(), (long double)func3());

    printf ("\nFunc 2.a:%13.4e\nFunc 2.b:%13.4e\nFunc 2.c:%13.4e\n", func21(),
            func22(), func23());
    printf ("\n");

    func31();
    printf ("\n");

    func41();
    func42();
    printf ("\n");

    func51();

    func61();

    func71();

    func81();

    // func91();
}

int main (int argc, char* argv[])
{
    clock_t timer;
    timer = clock();
    if (argc >= 2)
    {
        switch (argv[1][0])
        {
            case '1':
                printf ("Func 1.a:%13.4Le\nFunc 1.b:%13.4Le\nFunc 1.c:%13.4Le\n",
                        ldexpl ((long double)1., -func1()), (long double)func2(), (long double)func3());
                break;

            case '2':
                printf ("Func 2.a:%13.4e\nFunc 2.b:%13.4e\nFunc 2.c:%13.4e\n",
                        func21(), func22(), func23());
                break;

            case '3': func31(); break;

            case '4':
                func41();
                func42();
                break;

            case '5': func51(); break;

            case '6': func61(); break;

            case '7': func71(); break;

            case '8': func81(); break;

            case '9':
                // func91();
                break;

            default: all();
        }
    }
    else { all(); }

    timer -= clock();
    printf ("%.5f seconds\n", ((double) -timer) / CLOCKS_PER_SEC);
    return 0;
}

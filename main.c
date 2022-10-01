#include "lib.h"

void all (void)
{
    printf ("Func 1.a:%13.4e\nFunc 1.b:%13.4e\nFunc 1.c:%13.4e\n",
            ldexp (1., -func1()),
            func2(),
            func3());

    printf ("\nFunc 2.a:%13.4e\nFunc 2.b:%13.4e\nFunc 2.c:%13.4e\n",
            func21(),
            func22(),
            func23());
    printf ("\n");

    func31();
    printf ("\n");

    func41();
    func42();
    printf ("\n");

    func51();

    func61();
}

int main (int argc, char* argv[])
{
    if (argc >= 2)
    {
        switch (argv[1][0])
        {
            case '1':
                printf ("Func 1.a:%13.4e\nFunc 1.b:%13.4e\nFunc 1.c:%13.4e\n",
                        ldexp (1., -func1()),
                        func2(),
                        func3());
                break;

            case '2':
                printf ("Func 2.a:%13.4e\nFunc 2.b:%13.4e\nFunc 2.c:%13.4e\n",
                        func21(),
                        func22(),
                        func23());
                break;

            case '3':
                func31();
                break;

            case '4':
                func41();
                func42();
                break;

            case '5':
                func51();
                break;

            case '6':
                func61();
                break;

                /*
                            case '7':

                                break;

                            case '8':

                                break;
                */
            default:
                all();
        }
    }
    else
    {
        all();
    }

    return 0;
}

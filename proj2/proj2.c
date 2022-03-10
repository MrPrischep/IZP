//  Projekt 2
//  Dmitrii Kozhevnikov
//  xkozhe00
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

const double I0 = 1e-12;    //  value I0
const double Ut = 0.0258563;    //  value Ut

//  vypocet Ip
double funkceIp(double Up) 
{
    return I0 * (exp(Up/Ut) - 1);
}

//  controla value Up
int funkceUp(double Up) 
{
    if (Up < 0)
    {
        return -1;
    }
    else if (Up == 0)
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

//  controla Ip - Ir = 0
double controla(double Up, double U0, double R)
{
    return I0 * (exp(Up/Ut) - 1) - (U0-Up)/R;   
}

//  vypocet Up
double diode(double U0, double R, double EPS)
{
    double start = 0;
    double finish = U0;
    double Up;
    //  controla EPS > 1e-14
    if (funkceUp(controla(start, U0, R)) == 0) 
    {
        return 0;
    }
    else if (funkceUp(controla(finish, U0, R)) == 0) 
    {
        return U0;
    }
    //  metoda puleni intervalu
    while (fabs(finish - start) > EPS)
    {
        Up = (start + finish)/2;    //  middle of section
        if (Up == finish)
        {
            return Up;
        }
        if (funkceUp(controla(start, U0, R)) != funkceUp(controla(Up, U0, R)))
        {
            finish = Up;
        }
        else
        {
            start = Up;
        }
    }
    return Up;
}

int main (int argc, char *argv[])
{
    //  control, how much arguments we have
    if (argc == 4)
    {
        char *argv1;    //  argument for control value argv[1]
        char *argv2;    //  argument for control value argv[2]
        char *argv3;    //  argument for control value argv[3]
        double U0 = strtod(argv[1], &argv1);    //  value U0
        double R = strtod(argv[2], &argv2); //  value R
        double EPS = strtod(argv[3], &argv3);   //  value EPS
        //  control of entered values
        if (strcmp(argv1, "") || strcmp(argv2, "") || strcmp(argv3, "") || U0 < 0  || R < 0) 
        {
            fprintf(stderr, "%s\n", "Error: Invalid arguments!");
            return 1;
        }
        if (R == 0) 
        {
            fprintf(stderr, "%s\n", "Error: R must be higher than 0");
            return 1;
        }
        else if (U0 == 0) 
        {
            printf("Up=%d V\nIp=%d A\n", 0, 0);
        }
        else 
        {
            printf("Up=%g V\nIp=%g A\n",  diode(U0, R, EPS), funkceIp(diode(U0, R, EPS)));
        }
    }
    else
    {
        fprintf(stderr, "%s\n", "Error: Add only three arguments");
    }
    return 0;
}
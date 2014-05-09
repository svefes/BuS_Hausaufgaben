#include <stdio.h>
#include <stdlib.h>

long long strtonum(const char *nptr, long long minval, long long maxval, const char **errstr);

int fak(int n)
{
        if(n > 1)
        {
            return n * fak(n-1);
        }
        else
        {
            return 1;
        }
}

float euler(int depth)
{
    if(depth >= 1)
    {
        printf("Mach ich\n");
        return 1/fak(depth) + euler(depth - 1);
    }
    else
    {
        return 1;
    }
}

int main(int argc, char** argv)
{
    int depth;
    float euler_zahl;
    
    depth = strtol(argv[1], (char**)NULL, 10);
    euler_zahl = euler(depth);
    
    printf("Euler mir Genauigkeit %d: %f\n",depth, euler_zahl);
    
    return 0;
}

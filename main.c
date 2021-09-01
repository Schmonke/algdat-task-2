#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int ITERS = 1000000;

typedef double pow_func(double c, int n);

// Function for calculating the power of a number using the C standard library.
// This algorithm uses a linear approach. The base number (x) is multiplied by itself until (n) is equal to zero.
double pow_lin(double x, int n)
{
    if (n == 0)
    {
        return 1;
    }
    return x * pow_lin(x, n - 1);
}

// Function for calculating the power of a number using the C standard library.
// This algorithm uses a logarithmic approach. The base number (x) is squared (TODO)
double pow_log(double x, int n)
{
    if (n == 0)
    {
        return 1;
    }
    int odd = n % 2;
    double m = odd * (x - 1) + 1;
    return m * pow_log(x * x, n / 2);
}

// Function for calculating the power of a number using the C standard library.
double pow_libc(double x, int n)
{
    return pow(x, n);
}

// Function for measuring time of a power function.
// Returns the time as the amount of nanoseconds per power function call.
int time_pow_func(double x, int n, pow_func powFunc, int iters, double *result)
{
    struct timespec start, end;

    if (clock_gettime(CLOCK_REALTIME, &start))
    {
        printf("gettime failure\n");
        return -1;
    }

    // Use a loop to ensure we get a consistent measurement
    for (int i = 0; i < iters; i++)
    {
        powFunc(x, n);
    }

    if (clock_gettime(CLOCK_REALTIME, &end))
    {
        printf("gettime failure\n");
        return -1;
    }

    *result = powFunc(x, n);

    // Print out the time.
    long long ns = (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    return ns / iters;
}

int main()
{
    for (int i = 1; i <= 1000000000; i *= 10)
    {
        double x, result;
        int n, ns;
        x = 1.0000000010;
        n = i;

        printf("\n%f^%d:\n", x, n);
        ns = time_pow_func(x, n, pow_libc, ITERS, &result);
        printf("   libc   (%04dns): %f\n", ns, result);
        ns = time_pow_func(x, n, pow_lin, ITERS, &result);
        printf("   linear (%04dns): %f\n", ns, result);
        ns = time_pow_func(x, n, pow_log, ITERS, &result);
        printf("   opti   (%04dns): %f\n", ns, result);
    }
}
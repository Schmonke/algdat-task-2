#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int MAX_ITERS = 1000000;
const int LINEAR_SKIP = 2000;

typedef double pow_func(double c, int n);

/**
 * Function for calculating the power of a number.
 * 
 * This algorithm uses a linear approach. The base number (x) is multiplied with
 * itself until n=0.
 * 
 * The complexity of the algorithm will be linear as the operations are performed
 * n+1 times. The worst-case complexity for this function will therefore be
 * O(n). The best-case complexity, for the case n=0, will be O(1), as no
 * iterations are needed for that case.
 */
double pow_lin(double x, int n)
{
    if (n == 0)
    {
        return 1;
    }
    return x * pow_lin(x, n - 1);
}

/** 
 * Function for calculating the power of a number.
 * 
 * This algorithm uses a logarithmic approach. The base number (x) is squared
 * and that number is thereafter divided by n div 2 recursively until n=0.
 * If n is an odd number, the result is then multiplied by x once again, as the
 * exponent is truncated under the division, and we must therefore account for
 * the remainder. 
 * 
 * The complexity of the algorithm can be determined using mathematical analysis.
 * The exponent that is given to the function is divided by 2. The amount of
 * iterations until 1 is reached is therefore given by ceil(log2(n)). Since
 * the algorithm iterates until 0, we have to use ceil(log2(n)) + 1.
 * We can therefore conclude that in the worst case, the algorithm has
 * logarithmic complexity of O(log n). The best-case complexity, for the
 * case n=1, the complexity is Ω(1). 
 */
double pow_log(double x, int n)
{
    if (n == 0)
    {
        return 1;
    }
    int odd = n % 2;
    double m = odd * (x - 1) + 1; // odd => m = x, !odd => m = 1
    return m * pow_log(x * x, n / 2);
}

/**
 * Function for calculating the power of a number.
 * 
 * This function uses the C standard library to perform the calculation.
 */
double pow_libc(double x, int n)
{
    return pow(x, n);
}

/**
 * Function for measuring time of a power function.
 * Returns the time as the amount of nanoseconds per power function call.
 */
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
    printf("3^14\n");
    printf("   libc: %f\n", pow_libc(3, 14));
    printf("   lin:  %f\n", pow_lin(3, 14));
    printf("   log:  %f\n", pow_log(3, 14));

    printf("\n2^10\n");
    printf("   libc: %f\n", pow_libc(2, 10));
    printf("   lin:  %f\n", pow_lin(2, 10));
    printf("   log:  %f\n", pow_log(2, 10));

    printf("\n##### Performance Measurement #####\n");

    for (int i = 1; i <= 1000000000; i *= (i % 2 ? 3 : 10))
    {
        double x, result;
        int n, ns;
        x = 1.0000000010;
        n = i;

        printf("\n%.10f^%d:\n", x, n);
        ns = time_pow_func(x, n, pow_libc, MAX_ITERS, &result);
        printf("   libc   (%04dns): %f\n", ns, result);
        if (i > LINEAR_SKIP)
        {
            printf("   linear ( skip ): skipped\n");
        }
        else
        {
            ns = time_pow_func(x, n, pow_lin, MAX_ITERS, &result);
            printf("   linear (%04dns): %f\n", ns, result);
        }
        ns = time_pow_func(x, n, pow_log, MAX_ITERS, &result);
        printf("   log    (%04dns): %f\n", ns, result);
    }
}
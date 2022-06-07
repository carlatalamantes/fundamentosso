#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

//#define ITERATIONS 200000000000
#define ITERATIONS 1000

int main()
{
    // Time variables
    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    struct timeval ts;

    // Leibniz variables
    long double pi = 1.0;
    double denominator;
    long int i;

    // Getting start time
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    // Leibniz logic
    for (i = 0; i < ITERATIONS; i++)
    {
        denominator = i * 2 + 3;
        if (i % 2 == 0)
        {
            pi -= (1 / denominator);
        }
        else
        {
            pi += (1 / denominator);
        }
    }

    pi *= 4;
    printf("PI number is %Lf \n", pi);

    // Getting end time
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec;
    elapsed_time = stop_ts - start_ts;
    printf("Total exec time is %lld seconds\n", elapsed_time);

    return 1;
}
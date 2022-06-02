#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

//#define ITERATIONS 200000000000
#define ITERATIONS 1000
#define NTHREADS 4

void *tfunc(void *args)
{
    // Leibniz variables
    long double pi = 1.0;
    double denominator;
    long int i;

    // Thread variables
    int nthread = *((int *)args);
    long long start = (ITERATIONS / NTHREADS) * nthread;
    long long end = (ITERATIONS / NTHREADS) * (nthread + 1);
    int j;

    //printf("Hilo %d. Itervalo %lld - %lld\n",nthread,start,end);
    // Thread logic
    for (j = start; j < end; j++)
    {
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
    }

    pi *= 4;
    printf("PI number is %Lf \n", pi);
}

int main()
{
    // Time variables
    long long start_ts;
    long long stop_ts;
    long long elapsed_time;
    struct timeval ts;

    // Threads variables
    int i;
    int args[NTHREADS];
    pthread_t tid[NTHREADS];

    // Getting start time
    gettimeofday(&ts, NULL);
    start_ts = ts.tv_sec;

    // Creating threads
    for (i = 0; i < NTHREADS; i++)
    {
        args[i] = i;
        pthread_create(&tid[i], NULL, tfunc, &args[i]);
    }

    // Waiting for threads
    for (i = 0; i < NTHREADS; i++)
    {
        args[i] = i;
        pthread_join(tid[i], NULL);
    }

    // Getting end time
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec;
    elapsed_time = stop_ts - start_ts;
    printf("Total exec time is %lld seconds\n", elapsed_time);

    return 1;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>

//#define ITERATIONS 200000000000
#define ITERATIONS 1000
#define NTHREADS 4

long double pi_sum[NTHREADS];
double denominator=0;

void *tfunc(void *args)
{
    // Leibniz variables
    long double pi = 1.0;
    long int i;
    long int j;

    // Thread variables
    int nthread = *((int *)args);
    long long start = (ITERATIONS / NTHREADS) * nthread;
    long long end = (ITERATIONS / NTHREADS) * (nthread + 1);

    for (i = start; i < end; i++)
    {
        // Leibniz logic
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
       
    pi_sum[nthread] = pi;
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

    //Pi variable
    long double final_pi=0.0;

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

    // Getting pi
    for (i = 0; i < NTHREADS; i++)
    {
        //printf("%Lf \n",final_pi);
       final_pi=final_pi+pi_sum[i];
    }
    
    printf("PI number is %Lf \n",final_pi);

    // Getting end time
    gettimeofday(&ts, NULL);
    stop_ts = ts.tv_sec;
    elapsed_time = stop_ts - start_ts;
    printf("Total exec time is %lld seconds\n", elapsed_time);

    return 1;
}
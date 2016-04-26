#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include <sched.h>
#include <signal.h>

#ifdef __MACH__

    #include <pthread.h>
    #define sched_setscheduler pthread_setschedparam

#endif

#include "timing_mach.h"

/* in source folder, build and run with command:
   clang timing*.c -o timing_test && ./timing_test
*/

int main() {

    struct sched_param pri_param;
    pri_param.sched_priority = sched_get_priority_max (SCHED_FIFO);
    sched_setscheduler (0, SCHED_FIFO, &pri_param);

    int64_t epoch;
    struct timespec before, after;
    double sum, elapsed, waittime;
    unsigned u;

    /* initialize mach timing */
    #ifdef __MACH__
    timing_mach_init();
    #endif

    /* display epoch time */
    clock_gettime(CLOCK_REALTIME, &before);
    epoch = (int64_t) before.tv_sec;
    printf("\nEpoch Time is %" PRId64 " seconds\n", epoch);

    /* time something */
    printf("\nPerforming some math operations (sum) ...\n");
    clock_gettime(CLOCK_MONOTONIC, &before);
    sum = 0.0;
    for(u = 1; u < 100000000; u++){
        sum += 1./u/u;
    }
    clock_gettime(CLOCK_MONOTONIC, &after);
    /* print result */
    printf("Done. (result is: sum = %g)\n", sum);
    timespec_monodiff_rml (&before, &after);
    elapsed = timespec2secd(&before);
    printf("Elapsed Time = %e s\n", elapsed);

    /* absolute time nanosleep */
    after.tv_nsec = 0;
    after.tv_sec = 2;
    waittime = timespec2secd(&after);
    printf("\nWait %g seconds ... \n", waittime);
    clock_gettime(CLOCK_MONOTONIC, &before);
    timespec_monoadd(&after, &before);
    clock_nanosleep_abstime(&after);
    clock_gettime(CLOCK_MONOTONIC, &after);
    printf("Done waiting.\n");
    /* print result */
    timespec_monodiff_rml (&before, &after);
    elapsed = timespec2secd(&before);
    printf("Elapsed Time = %e s\n", elapsed);

    /* interval timer */
    #define NUM_ITERS 10
    double time_step = 0.0005;
    /* work variables */
    double deltas[NUM_ITERS];
    int k = 0;
    struct timespec ts_step, ts_target;
    secd2timespec(&ts_step, time_step);
    printf("\nInterval timer with step size of %g seconds ...\n", time_step);
    clock_gettime(CLOCK_MONOTONIC, &before);
    /* start timer - target will be set to time_step seconds after current time */
    itimer_start (&ts_target, &ts_step);
    for (k = 0; k < NUM_ITERS; k++) {
        /* run interval timer (nanosleep remaining time to target and increment) */
        itimer_step (&ts_target, &ts_step);
        /* check change in time from previous iteration */
        clock_gettime(CLOCK_MONOTONIC, &after);
        timespec_monodiff_rml (&before, &after);
        deltas[k] = timespec2secd(&before);
        before.tv_nsec = after.tv_nsec;
        before.tv_sec = after.tv_sec;
    }
    /* print results */
    for (k = 0; k < NUM_ITERS; k++) {
        printf("iteration %d >>> waited %e seconds\n", k, deltas[k]);
    }
    printf("Finished interval timer.\n");

    /* display epoch time */
    clock_gettime(CLOCK_REALTIME, &before);
    epoch = (int64_t) before.tv_sec;
    printf("\nEpoch Time is %" PRId64 " seconds \n", epoch);

}

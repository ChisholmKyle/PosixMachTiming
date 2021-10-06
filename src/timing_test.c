#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <pthread.h>

#include "timing_mach.h"

/* interval timer */
#define NUM_ITERS 20
#define INTERVAL_TIME_STEP 0.0005

/* in source folder, build and run with command:
   clang timing*.c -o timing_test && ./timing_test
*/

int main() {

    /* set high priority FIFO scheduling */
    struct sched_param pri_param;
    int sched_policy;
    int max_priority;
    pthread_getschedparam (pthread_self (), &sched_policy, &pri_param);
    max_priority = sched_get_priority_max (SCHED_FIFO);
    pri_param.sched_priority = max_priority - 1;
    pthread_setschedparam (pthread_self (), SCHED_FIFO, &pri_param);

    pthread_getschedparam (pthread_self (), &sched_policy, &pri_param);
    printf("\nSched policy is %s\n", (sched_policy == SCHED_FIFO ? "SCHED_FIFO" : "SCHED_OTHER"));
    printf("Sched priority is %d/%d\n", pri_param.sched_priority, max_priority);

    /* timing examples */
    int64_t epoch;
    int status;
    struct timespec res, before, after;
    double sum, elapsed, waittime;
    unsigned u;
    
    /* using clock_getres before initializing produces an intelligible error */
    status = clock_getres(CLOCK_MONOTONIC, &res);
    printf("\nStatus from clock_getres before init is %d and errno is %d\n", status, errno);

    /* initialize mach timing */
    timing_mach_init();

    /* display epoch time */
    clock_gettime(CLOCK_REALTIME, &before);
    epoch = (int64_t) before.tv_sec;
    printf("\nEpoch Time is %" PRId64 " seconds\n", epoch);
    
    /* get clock resolution */
    clock_getres(CLOCK_REALTIME, &res);
    printf("\nResolution of realtime clock: tv_sec %ld tv_nsec %ld", res.tv_sec, res.tv_nsec);
    clock_getres(CLOCK_MONOTONIC, &res);
    printf("\nResolution of monotonic clock: tv_sec %ld tv_nsec %ld\n", res.tv_sec, res.tv_nsec);
    
    /* null pointer should not cause an error */
    clock_getres(CLOCK_MONOTONIC, NULL);
    clock_gettime(CLOCK_MONOTONIC, NULL);

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
    double time_step = INTERVAL_TIME_STEP;
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
        /* advance timer */
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

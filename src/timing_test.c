#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

#include "timing_mach.h"
#include <stdio.h>
#include <inttypes.h>

/* in source folder, build and run with command:
   gcc timing*.c -o timing_test && ./timing_test
*/

int main(){
    struct timespec before, after;
    /* initialize mach timing */
    #ifdef __MACH__
    timing_mach_init();
    #endif
    /* display epoch time */
    clock_gettime(CLOCK_REALTIME, &before);
    int64_t epoch = (int64_t) before.tv_sec;
    printf("Epoch Time is %" PRId64 " seconds \n", epoch);
    /* time something */
    clock_gettime(CLOCK_MONOTONIC, &before);
    double sum = 0.0;
    unsigned u;
    for(u = 1; u < 100000000; u++)
    sum += 1./u/u;
    clock_gettime(CLOCK_MONOTONIC, &after);
    /* print result */
    timespec_monodiff (&before, &after);
    double elapsed = timespec2secd(&before);
    printf("sum = %e\n", sum);
    printf("elapsed = %e s\n", elapsed);
    /* absolute time nanosleep */
    after.tv_nsec = 0;
    after.tv_sec = 2;
    double waittime = timespec2secd(&after);
    clock_gettime(CLOCK_MONOTONIC, &before);
    timespec_monoadd(&after, &before);
    printf("wait %g seconds...\n", waittime);
    clock_nanosleep_abstime(&after, NULL);
    clock_gettime(CLOCK_MONOTONIC, &after);
    printf("Done waiting.\n");
    /* print result */
    timespec_monodiff (&before, &after);
    elapsed = timespec2secd(&before);
    printf("elapsed = %e s\n", elapsed);
    /* display epoch time */
    clock_gettime(CLOCK_REALTIME, &before);
    epoch = (int64_t) before.tv_sec;
    printf("Epoch Time is %" PRId64 " seconds \n", epoch);
}

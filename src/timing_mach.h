#include <time.h>

#define TIMING_GIGA (1000000000)
#define TIMING_NANO (1e-9)

extern double timespec2secd(const struct timespec *ts_in);
extern void timespec_monodiff(struct timespec *ts_out,
                              const struct timespec *ts_in);
extern void timespec_monoadd(struct timespec *ts_out,
                             const struct timespec *ts_in);

#ifdef __MACH__
/* ******** */
/* __MACH__ */

/* Defines - emulate POSIX
 * only CLOCK_REALTIME and CLOCK_MONOTONIC are emulated
 */
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

/* typdefs and struct definitions - emulate POSIX */
typedef int clockid_t;

/* initialize mach timing */
int timing_mach_init (void);

/* clock_gettime - emulate POSIX */
int clock_gettime(const clockid_t id, struct timespec *tspec);
/* clock_nanosleep for CLOCK_MONOTONIC and TIMER_ABSTIME */
int clock_nanosleep_abstime(const struct timespec *req,
                            struct timespec *rem);

/* __MACH__ */
/* ******** */
#else
/* ***** */
/* POSIX */

/* clock_nanosleep for CLOCK_MONOTONIC and TIMER_ABSTIME */
#define clock_nanosleep_abstime(req,rem) \
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, (req), (rem))

/* POSIX */
/* ***** */
#endif

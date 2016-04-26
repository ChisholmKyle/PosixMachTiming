# PosixMachTiming #

A tiny program to partially emulate POSIX `clock_nanosleep()` and
`clock_gettime()` for mac OS X.

- The code should work with both C99 (some functions are inlined) and ANSI C
- Only `CLOCK_REALTIME` and `CLOCK_MONOTONIC` are emulated for `clock_gettime()`.
- `clock_nanosleep()` is actually not emulated, but rather a a function called `clock_nanosleep_abstime(...)` is implemented which is equivalent to a POSIX call `clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ...)`.
- A simple interval timer based on `clock_nanosleep_abstime()` is also implemented with two functions: `itimer_start()` and `itimer_step()`.

Please see test program and source files. There is no documentation for this code, but it does have comments and should not be too hard to figure out. If anyone wants to contribute, they may want to add tests with error checking and/or simple documentation. If any more features get thrown in, I think a proper API should be implemented.

## Build ##

There is an extremely simple test program in the [src](https://github.com/ChisholmKyle/PosixMachTiming/tree/master/src) folder. The test program outputs epoch time, times an operation, sleeps to a specified absolute time, and runs an interval timer at 2000Hz. You can compile and run from the command line. For example:

    clang timing*.c -o timing_test && ./timing_test

should output something like

    Epoch Time is 1461691145 seconds

    Performing some math operations (sum) ...
    Done. (result is: sum = 1.64493)
    Elapsed Time = 9.905245e-01 s

    Wait 2 seconds ...
    Done waiting.
    Elapsed Time = 2.003565e+00 s

    Interval timer with step size of 0.0005 seconds ...
    iteration 0 >>> waited 5.882610e-04 seconds
    iteration 1 >>> waited 5.181580e-04 seconds
    iteration 2 >>> waited 5.100900e-04 seconds
    iteration 3 >>> waited 4.999900e-04 seconds
    iteration 4 >>> waited 5.003040e-04 seconds
    iteration 5 >>> waited 5.007280e-04 seconds
    iteration 6 >>> waited 4.996460e-04 seconds
    iteration 7 >>> waited 5.009060e-04 seconds
    iteration 8 >>> waited 5.001000e-04 seconds
    iteration 9 >>> waited 4.995620e-04 seconds
    Finished interval timer.

    Epoch Time is 1461691148 seconds

Note that in this code example, scheduling is applied to maximum priority so that the timer performs reliably.


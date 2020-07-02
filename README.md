# PosixMachTiming #

A tiny program to implement a simple timer, some basic utility functions, and partially emulate POSIX `clock_nanosleep()` and
`clock_gettime()` for OS X or macOS. The code also works POSIX compliant systems, including Linux.

- The code uses inline function and requires C99
- Mac OS Sierra (10.12) and above implement `clock_gettime()` and does not require emulation. However `clock_nanosleep()` is still not implemented in any version of macOS or OS X.
- Only `CLOCK_REALTIME` and `CLOCK_MONOTONIC` are emulated for `clock_gettime()`.
- `clock_nanosleep()` is not emulated, but rather a a function called `clock_nanosleep_abstime(...)` is implemented which is equivalent to a POSIX call `clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, ...)`.
- A simple interval timer based on `clock_nanosleep_abstime()` is also implemented with two functions: `itimer_start()` and `itimer_step()`.

Please see test program and source files. If anyone wants to contribute, they may want to add tests with error checking and/or simple documentation. If any more features get thrown in, I think a proper API should be implemented.

## Build ##

There is an extremely simple example program in the [src](https://github.com/ChisholmKyle/PosixMachTiming/tree/master/src) folder. This program outputs epoch time, times an operation, sleeps to a specified absolute time, and runs an interval timer at 2000Hz. You can compile and run from the command line. For example:

    clang -std=c99 timing*.c -o timing_test && ./timing_test

should output something like

``` text
Sched policy is SCHED_FIFO
Sched priority is 46/47

Status from clock_getres before init is -1 and errno is 45

Epoch Time is 1593677616 seconds

Resolution of realtime clock: tv_sec 0 tv_nsec 1
Resolution of monotonic clock: tv_sec 0 tv_nsec 1

Performing some math operations (sum) ...
Done. (result is: sum = 1.64493)
Elapsed Time = 8.574453e-01 s

Wait 2 seconds ...
Done waiting.
Elapsed Time = 2.000412e+00 s

Interval timer with step size of 0.0005 seconds ...
iteration 0 >>> waited 5.876090e-04 seconds
iteration 1 >>> waited 5.887230e-04 seconds
iteration 2 >>> waited 4.898970e-04 seconds
iteration 3 >>> waited 4.884590e-04 seconds
iteration 4 >>> waited 5.181490e-04 seconds
iteration 5 >>> waited 4.976460e-04 seconds
iteration 6 >>> waited 4.704730e-04 seconds
iteration 7 >>> waited 5.430580e-04 seconds
iteration 8 >>> waited 4.815320e-04 seconds
iteration 9 >>> waited 4.065470e-04 seconds
iteration 10 >>> waited 5.681740e-04 seconds
iteration 11 >>> waited 5.301990e-04 seconds
iteration 12 >>> waited 4.931940e-04 seconds
iteration 13 >>> waited 4.469870e-04 seconds
iteration 14 >>> waited 5.583800e-04 seconds
iteration 15 >>> waited 4.978350e-04 seconds
iteration 16 >>> waited 4.981900e-04 seconds
iteration 17 >>> waited 5.003510e-04 seconds
iteration 18 >>> waited 5.005460e-04 seconds
iteration 19 >>> waited 4.995940e-04 seconds
Finished interval timer.

Epoch Time is 1593677619 seconds
```

Note that in this code example, scheduling is applied to a high priority so that the timer performs reliably.


## Ways to Contribute ##

This tiny repo might be useful as a very performant and lightweight timer to get up and running on any POSIX system. Possible ways to turn this into a proper module are:

- Add Windows functions with [MinGW](http://www.mingw.org/), [Msys2](http://www.msys2.org/), and [high-resolution Windows timers](https://msdn.microsoft.com/en-us/library/windows/desktop/dn553408).
- Date and time formatted printing and conversions
- Unit tests (eg. Unity)
- Documentation (eg. doxygen)

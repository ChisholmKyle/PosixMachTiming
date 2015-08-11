# PosixMachTiming #
A tiny program to partially emulate POSIX `clock_nanosleep()` and 
`clock_gettime()` for mac OS X.

## Build ##
There is an extremely simple test program in the 
[src](https://github.com/ChisholmKyle/PosixMachTiming/tree/master/src) folder
you can compile and run from the command line. For example:

    clang timing*.c -o timing_test && ./timing_test

should output something like

    Epoch Time is 1439262872 seconds
    sum = 1.644934e+00
    elapsed = 1.000153e+00 s
    wait 2 seconds...
    Done waiting.
    elapsed = 2.000153e+00 s
    Epoch Time is 1439262875 seconds

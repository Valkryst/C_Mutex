The foci of this repository are the [mutex.c](https://github.com/Valkryst/C_Mutex/master/src/mutex.c)
and [mutex.h](https://github.com/Valkryst/C_Mutex/master/src/mutex.h)
files. They simplify the creation, destruction, and use of
[mutex locks](https://en.wikipedia.org/wiki/Lock_(computer_science)).

## Compiling

Add `mutex.c` and `mutex.h` to your project, and add
[-pthread flag](https://stackoverflow.com/a/2127819/13279616) to your compile
flags.

See the provided [Makefile](https://github.com/Valkryst/C_Mutex/master/Makefile)
as a reference.

## Dependencies

For development purposes, `mutex.c` depends on my
[C_Logging](https://github.com/Valkryst/C_Logging) library. By replacing all
calls to `printError` in `mutex.c`, you can remove this dependency.
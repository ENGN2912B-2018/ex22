# Classroom example code from Lectures 22-24 - C++11 Standard Threads

Since the C++11 standard was released, thread support has been a part of the C++ built-in libraries.  This support had been expanded upon in C++14 and C++17, but the 2011 implementation has the majority of the features necessary for most multi-threaded applications.  The examples in this repository have been adapted from the text "C++ Concurrency in Action" by Anthony Williams (Manning, 2nd Ed., 2019).

## Building the threaded examples

### GNU Compiler
You can compile the code in these examples using the standard g++ compiler options:

```
g++ -std=c++11 -lpthread -o <executable_name> <filename>.cpp
```

Note that although we are not using pthreads directly, GCC must be able to link to the POSIX threads for the underlying system (linux in this case).  Some compilers automatically link to these libraries and explicit linking may not be necessary.  Using the verbose compiler option, `-v`, you can investigate further.

### CMake build
There is also a CMakeLists.txt file that allows you to build each example.  Using the standard CMake out of source build:

```
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Debug ..
make
```

Upon logging into the CCV, you will notice a message stating the number of cores that are available to you.  Currently, the accounts for ENGN2912B have been setup as "exploratory" accounts with access to a maximum of 16 cores.  Recall that although you can compile your code on the login node, you should run compute intensive executables on the compute nodes. Request a compute node by logging in with the interactive mode (`interact -n <ncores>`) or by running a batch file (`sbatch`).  This will avoid impacting other users on the login node, Oscar.  It will also ensure that you have more repeatable profiling results.

## Profiling multi-threaded programs

### Linux 'time' utility

Linux contains a simple `time` command that you can use to get the approximate execution time.  This is probably the simplest way to get the ballpark estimate of your execution time; however, this utility simply computes the total execution time regardless of what else the CPU is doing.  It will generally overestimate the execution time for multi-threaded applications and may produce inconsistent results.

```
time ./main2
```

This will return three numbers that consist of 1) the total time taken to run the executable, 2) the total time spent executing the program (user mode), and 3) the total time spent on operating system overhead (kernel mode).

```
real	0m1.763s
user	0m1.750s
sys     0m0.004s
```

For multi-threaded code, you'll notice that the 'user' time includes cumulative time spent on all threads, so a multi-core CPU can report a 'user' time greater than the 'real' time since it was executing threads in parallel.

This utility is easy to use for back of the envelope benchmarking and profiling, but it should not be used for anything that requires precision and repeatable results.

### C++11 Chrono library

The system clock can be invoked directly within the C++ code.  Since C++11, the `chrono` library provides a standard way to accurately measure the execution time of a section of code.

```c++
auto start = chrono::steady_clock::now();

//  Code segment to be timed

auto end = chrono::steady_clock::now();

// Store the relative time difference between start and end
auto diff = end - start;

// Output the execution time in various units
cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
cout << chrono::duration <double, micro> (diff).count() << " us" << endl;
cout << chrono::duration <double, nano> (diff).count() << " ns" << endl;
```

This method will allow you to get the actual execution time for any section(s) of your code you wish to run, rather than for the entire program as `time` generates.  This method reads the system clock in real time.  As such, it does not compute the execution time specific to each thread.  It will, however, allow you to profile only a segment of your code if you are trying to accelerate it.

Note that if you don't have c++11 compiler support, you can still use the Boost Posix_Time library or the C standard library `time.h`, which includes several macros for timing C programs.

### GNU Profiler (gprof)

As seen in single-threaded executables from earlier clossroom examples, the GNU Profiler can be used to investigate the detailed execution time of the program and each function call.  All that is needed is to build the executable with GCC's `-pg` option.  You can also direct CMake to include these options in your `Debug` target (i.e., when using the `-DCMAKE_BUILD_TYPE=Debug` argument).

```
g++ -std=c++11 -pg -lpthread -o main1 main1.cpp
```

Simply run the executable after it compiles to create the profiling data, `gmon.out`.

```
./main1
```

Then analyze the profiler output, run

```
gprof ./main2 gmon.out > analysis.txt
```

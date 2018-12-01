# Classroom example code from Lectures 22-24 - C++11 Standard Threads

Since the C++11 standard was released, thread support has been a part of the C++ built-in libraries.  This support had been expanded upon in C++14 and C++17, but the 2011 implementation has the majority of the features necessary for most multi-threaded applications.  The examples in this repository have been adapted from the text "C++ Concurrency in Action" by Anthony Williams (Manning, 2nd Ed., 2019).

## Building the threaded examples

You can compile the code in these examples using:

```
g++ -std=c++11 -lpthread -o <executable_name> <filename>.cpp
```

Note that although we are not using pthreads directly, GCC must be able to link to the POSIX threads for the underlying system (linux in this case).  Some compilers automatically link to these libraries and explicit linking may not be necessary.  Using the verbose compiler option, `-v`, you can investigate further.

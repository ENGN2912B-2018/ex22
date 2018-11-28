# Classroom example code from Lecture 22 - C++11 Standard Threads

You can compile the code in these examples using:

```
g++ -std=c++11 -lpthread -o <executable_name> <filename>.cpp
```

Note that although we are not using pthreads, the underlying system (CCV in our case) must be able to link to the POSIX Threads in order to build.

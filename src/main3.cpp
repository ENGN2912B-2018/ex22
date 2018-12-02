// example adapted from Williams "C++ Concurrency in Action" (Manning, 2nd Ed., 2019)
// listing_2.8.cpp

#include <thread>
#include <numeric>      // provides accumulate (serial)
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <cassert>
#include <chrono>

namespace chrono = std::chrono;

//#define DEBUG
#define CHRONO

template<typename Iterator,typename T>
struct accumulate_block {
    void operator()(Iterator first,Iterator last,T& result) {
        result = std::accumulate(first,last,result);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first,Iterator last,T init) {

    // get vector length
    unsigned long const length = std::distance(first,last);
    #ifdef DEBUG
    std::cout << "Vector contains " << length << " elements" << std::endl;
    #endif //DEBUG

    // return with initial value if length is 0
    if(!length)
        return init;

    // define min/max values
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length+min_per_thread-1) / min_per_thread;

    // get number of available processing cores
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    #ifdef DEBUG
    std::cout << "Found " << hardware_threads << " processor cores" << std::endl;
    #endif //DEBUG

    // determine number of threads (default to 2 if we can't tell)
    unsigned long const num_threads =
        std::min(hardware_threads!=0 ? hardware_threads : 2, max_threads);
    #ifdef DEBUG
    std::cout << "Using " << num_threads << " threads" << std::endl;
    #endif //DEBUG

    // determine block size
    unsigned long const block_size = length / num_threads;
    #ifdef DEBUG
    std::cout << "Block size is " << block_size << " elements" << std::endl;
    #endif //DEBUG

    // initialize vectors
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads-1);

    // launch a new thread for each block
    Iterator block_start = first;
    for(unsigned long i=0; i<(num_threads-1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);    // move block_end to start of next block
        threads[i] = std::thread(               // launch new thread, store object in vector of threads
            accumulate_block<Iterator,T>(),     // thread executes with function object
            block_start, block_end, std::ref(results[i]));  // pass indices by value, vector by reference
        block_start = block_end;
    }

    // add remaining values in main thread
    accumulate_block<Iterator,T>()(block_start,last,results[num_threads-1]);

    // join each thread to main - block execution until all threads are complete
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    // sum results of each thread and return
    auto res = std::accumulate(results.begin(), results.end(), init);
    return res;
}

int main(int argc, char* argv[])
{
    // accept optional input parameter for vector size
    int vSize = 1000;
    if (argc > 1)
        vSize = atoi(argv[1]);
    assert(vSize);      // verify positive integer entered

    // populate vector with integers
    std::vector<int> vi;
    for(int i=0; i<vSize; ++i) {
        vi.push_back(10);
    }

    // run serial accumulation (and time it)
    auto start = chrono::steady_clock::now();
    int ser_sum = accumulate(vi.begin(), vi.end(), 5);
    auto end = chrono::steady_clock::now();

    #ifdef CHRONO
    std::cout << "Serial Elapsed Time = " << chrono::duration <double, std::milli> (end-start).count() << " ms" << std::endl;
    #endif //CHRONO

    std::cout << "Serial sum = " << ser_sum << "\n\n" << std::endl;

    // run parallel accumulation (and time it)
    start = chrono::steady_clock::now();
    int par_sum = parallel_accumulate(vi.begin(), vi.end(), 5);
    end = chrono::steady_clock::now();

    #ifdef CHRONO
    std::cout << "Parallel Elapsed Time = " << chrono::duration <double, std::milli> (end-start).count() << " ms" << std::endl;
    #endif // CHRONO

    std::cout << "Parallel sum = " << par_sum << std::endl;
}

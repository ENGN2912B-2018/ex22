// example adapted from Williams "C++ Concurrency in Action" (Manning, 2nd Ed., 2019)
// listing_2.7.cpp

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>

void do_work(unsigned id) {
	for (int j; j < 100000000; j++){};
	std::cout << id << std::endl;
}

int main() {
    std::vector<std::thread> threads;
    for(unsigned i=0;i<20;++i)
    {
        threads.push_back(std::thread(do_work,i));
    }
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));  // std::thread::join() would fail
}
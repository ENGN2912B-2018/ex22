// example adapted from Williams "C++ Concurrency in Action" (Manning, 2nd Ed., 2019)
// listing_2.7.cpp

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <cstdlib>

void do_work(unsigned id) {
	for (int j; j < 1000000000; j++){};
	std::cout << id << std::endl;
}

int main(int argc, char* argv[]) {
	unsigned N=10;
	if (argc > 1)
		N = atoi(argv[1]);
	
    std::vector<std::thread> threads;
    for(unsigned i=0;i<N;++i)
    {
        threads.push_back(std::thread(do_work,i));
    }
    std::for_each(threads.begin(),threads.end(),
        std::mem_fn(&std::thread::join));  // std::thread::join() would fail
}

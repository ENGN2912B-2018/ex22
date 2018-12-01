cmake_minimum_required (VERSION 3.2)

# create the project (must have a name)
project (ex22_threads)

# add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg -Wall -std=c++11")
set(CMAKE_BUILD_TYPE "Debug")

# create the executable
add_executable(main1 src/main1.cpp)
add_executable(main2 src/main2.cpp)
add_executable(main3 src/main3.cpp)

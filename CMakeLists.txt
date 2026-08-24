cmake_minimum_required(VERSION 4.2)
project(primecirclepacking)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "-O1")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(primecirclepacking main.cpp
        randompack.h
        circles.h
        view.h
        primes.h
        progressivepack.h
        pack.h
)

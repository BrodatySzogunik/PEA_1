cmake_minimum_required(VERSION 3.21)
project(BruteForce)

set(CMAKE_CXX_STANDARD 14)

add_executable(BruteForce main.cpp TravelingSalesman.cpp TravelingSalesman.h Timer.cpp Timer.h Test.cpp Test.h Structures/Array.cpp Structures/Array.h Structures/Path.cpp Structures/Path.h)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

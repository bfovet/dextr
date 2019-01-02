cmake_minimum_required(VERSION 3.13)
project(dextr)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_COMPILER /usr/local/bin/mpicc)
SET(CMAKE_CXX_COMPILER /usr/local/bin/mpicxx)

find_package(Boost 1.68.0 COMPONENTS mpi serialization program_options REQUIRED)

if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(dextr main.cpp)

if (Boost_FOUND)
    target_link_libraries(dextr ${Boost_LIBRARIES})
endif()

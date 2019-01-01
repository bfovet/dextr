cmake_minimum_required(VERSION 3.13)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_COMPILER /usr/local/bin/mpicc)
SET(CMAKE_CXX_COMPILER /usr/local/bin/mpicxx)

find_package(Boost 1.68.0 COMPONENTS mpi serialization program_options REQUIRED)

if (Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_executable(untitled main.cpp)

if (Boost_FOUND)
    target_link_libraries(untitled ${Boost_LIBRARIES})
endif()
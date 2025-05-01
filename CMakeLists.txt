cmake_minimum_required(VERSION 3.22)
project(so2_projekt1)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

add_executable(so2_projekt1 main.cpp
                            Car.hpp
                            Draw.hpp
                            Factory.hpp
                            Lift.hpp)

target_link_libraries(so2_projekt1 ${CURSES_LIBRARIES} Threads::Threads)
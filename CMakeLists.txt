cmake_minimum_required(VERSION 3.20)
project(musical_robot_dining C)

set(CMAKE_C_STANDARD 11)
link_libraries(pthread)

add_executable(dining-philosopher dining-philosopher.c dining-philosopher.h)
add_executable(reader-writer reader-writer.c reader-writer.h)

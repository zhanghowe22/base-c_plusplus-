# 规定该CMakeLists.txt适用的cmake最小版本
cmake_minimum_required(VERSION 3.10)

project(ring_queue)

add_executable(${PROJECT_NAME} src/ring_queue.cpp)
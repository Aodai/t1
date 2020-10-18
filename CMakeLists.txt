cmake_minimum_required(VERSION 3.0.0)
project(quiz VERSION 0.1.0)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
add_executable(quiz main.c)
target_link_libraries(quiz PRIVATE Threads::Threads)
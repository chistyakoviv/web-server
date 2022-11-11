cmake_minimum_required(VERSION 2.8...3.1 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 11)

project(multithreading_final)

# lots of warnings and all warnings as errors
add_compile_options(-Wall -Wextra -pedantic -Werror -std=c++11)

set(sources
    main.cpp
    utils.cpp
    server.cpp
    thread_pool.cpp
    response.cpp
    request.cpp
    log.cpp
)

set(headers
    utils.h
    server.h
    thread_pool.h
    queue.h
    response.h
    request.h
    log.h
)

add_executable(final ${sources} ${headers})

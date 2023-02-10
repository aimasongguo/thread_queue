cmake_minimum_required(VERSION 3.10)

project(thread_queue)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(clog pthread)

add_executable(
    thread_queue
    main.c
    thread_queue.c
    thread_queue.h
    test.c
    test.h
)

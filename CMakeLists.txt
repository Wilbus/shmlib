cmake_minimum_required(VERSION 3.10)
project(shmlib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")


add_library(${PROJECT_NAME} SHARED
    #${CMAKE_CURRENT_LIST_DIR}/shmlib/src/SharedBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shmlib/src/SharedRingBuffer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shmlib/src/SharedSemaphore.cpp
    ${CMAKE_CURRENT_LIST_DIR}/shmlib/src/SharedSemaphoreSentry.cpp
)
target_include_directories(shmlib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/shmlib/include
)

add_subdirectory(unit_tests)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wfatal-errors -Wunused-parameter -Wunused-variable -DLOG_FORMAT_VERIFICATION)

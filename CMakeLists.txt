cmake_minimum_required(VERSION 3.10.0)
project(openwalker)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")

add_subdirectory(dep/glm)

include_directories(src)
include_directories(dep/asio-1.12.2/include)

find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DASIO_STANDALONE)

link_libraries(glm)

add_library(openwalker STATIC ${SOURCES})
target_link_libraries(openwalker ${CMAKE_THREAD_LIBS_INIT})

add_executable(test ${TEST_SOURCES})
target_link_libraries(test openwalker)

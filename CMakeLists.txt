cmake_minimum_required(VERSION 3.10.0)
project(openwalker)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_subdirectory(dep/glm)

include_directories(src)

find_package(Boost 1.72 COMPONENTS system thread log log_setup REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
link_libraries(${Boost_LIBRARIES} glm)

add_executable(openwalker ${SOURCES})

cmake_minimum_required(VERSION 3.10.0)
project(openwalker)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE OW_F_LIB_SOURCES "ow_f_lib/src/*.cpp")
if(NOT OW_SKIP_TEST)
  file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
  file(GLOB_RECURSE TSTLIB_SOURCES "tstlib/*.cpp")
  file(GLOB_RECURSE PROXY_SOURCES "proxy/*.cpp")
endif()

add_subdirectory(dep/glm)

include_directories(dep/asio-1.16.1/include)
include_directories(include)
include_directories(ow_f_lib/include)

find_package(Threads)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
add_definitions(-DASIO_STANDALONE)

link_libraries(glm)

add_library(openwalker STATIC ${SOURCES})
target_link_libraries(openwalker ${CMAKE_THREAD_LIBS_INIT})

add_library(ow_f_lib STATIC ${OW_F_LIB_SOURCES})
target_link_libraries(ow_f_lib openwalker)

set(OW_LIBS openwalker ow_f_lib)

if(NOT OW_SKIP_TEST)
  add_library(tstlib STATIC ${TSTLIB_SOURCES})
  target_link_libraries(tstlib ${OW_LIBS})

  add_executable(test ${TEST_SOURCES})
  target_link_libraries(test ${OW_LIBS} tstlib)

  add_executable(ow_proxy ${PROXY_SOURCES})
  target_link_libraries(ow_proxy ${OW_LIBS} tstlib)
endif()

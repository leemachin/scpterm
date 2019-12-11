cmake_minimum_required(VERSION 3.15)
project(scpterm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(scpterm src/main.cpp)

target_link_libraries(scpterm ${SDL2_LIBRARIES})

cmake_minimum_required(VERSION 3.15)
project(scpterm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED SDL2_image>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} include)

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

add_executable(scpterm src/main.cpp src/timer.cpp src/timer.h)

target_link_libraries(scpterm ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})

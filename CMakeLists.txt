cmake_minimum_required(VERSION 3.8)
project(Solo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp)

include_directories(./Libs/Boost/)


add_subdirectory(./Network)

add_executable(Solo ${SOURCE_FILES})

target_link_libraries(Solo Network)


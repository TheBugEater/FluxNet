# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Project's name
project(FluxNet)

set(SOURCE_DIR "Source")
set(INC_DIR "Include")
set(SAMPLE_DIR "Sample")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_STANDARD 11)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

# The following folder will be included
include_directories(${INC_DIR})
include_directories(${INC_DIR}/FluxNet)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE INCLUDES ${INC_DIR}/*.h)

add_library(FluxNet ${SOURCES} ${INCLUDES})

target_link_libraries(FluxNet ws2_32)

file(GLOB SAMPLE ${SAMPLE_DIR}/*.cpp)
add_executable(Sample ${SAMPLE})
link_libraries(${CMAKE_BINARY_DIR})
target_link_libraries(Sample FluxNet)

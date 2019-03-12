# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.8)

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

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${SOURCE_DIR}/*.c*"
    "${INC_DIR}/*.h*"
)

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

add_library(FluxNet ${_source_list})

target_link_libraries(FluxNet ws2_32)

file(GLOB SAMPLE ${SAMPLE_DIR}/*.cpp)
add_executable(Sample ${SAMPLE})
link_libraries(${CMAKE_BINARY_DIR})
target_link_libraries(Sample FluxNet)

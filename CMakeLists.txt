# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# Project's name
project(FluxNet)

set(SOURCE_DIR "src")
set(INC_DIR "inc")
set(SAMPLE_DIR "sample")

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set (CMAKE_CXX_STANDARD 11)
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}")
file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${_src_root_path}/*.c*"
    "${_src_root_path}/*.h*"
)

add_library(MySources ${_source_list})

foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
    source_group("${_source_path_msvc}" FILES "${_source}")
endforeach()

file(GLOB SAMPLE ${SAMPLE_DIR}/*.cpp)
add_executable(Sample ${SAMPLE})
link_libraries(${CMAKE_BINARY_DIR})
target_link_libraries(Sample FluxNet)

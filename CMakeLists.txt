cmake_minimum_required(VERSION 3.10)
project(Raytraycing)

set(CMAKE_CXX_STANDARD 20)


# Set output directory for executables (both Debug and Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# Collect all source files that should be compiled as a executable
file(GLOB SOURCES "src/*.cpp")

# Specify include directory explicitly
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create executable
add_executable(raytracing main.cpp ${SOURCES})

# Link renderer_lib and main
target_include_directories(raytracing PRIVATE include)
cmake_minimum_required(VERSION 3.10)
project(IntroRayTraycing)

set(CMAKE_CXX_STANDARD 20)


# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# Collect all source files that should be compiled as separate executables
file(GLOB SOURCES "*.cpp")


# Collect utility source files separately
file(GLOB UTIL_SOURCES "src/*.cpp")

# Collect renderer source files separately
file(GLOB RENDER_SOURCES "src/renderer/*.cpp")

# Erstelle eine Renderer-Bibliothek
add_library(renderer_lib ${RENDER_SOURCES})

# Specify include directory explicitly
include_directories(${CMAKE_SOURCE_DIR}/include)

# Create an executable for each source file
foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(EXECUTABLE_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILE} ${UTIL_SOURCES})

    # link renderer_lib and main
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE renderer_lib)
    target_include_directories(${EXECUTABLE_NAME} PRIVATE include)
endforeach()
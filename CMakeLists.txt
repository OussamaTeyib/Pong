# Minimum CMake version required for this project
cmake_minimum_required(VERSION 3.5)

# Define the project name, version, and programming language
project(Pong VERSION 1.0.0 LANGUAGES C)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Define the directory for source files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# Gather all .c files in the source directory
file(GLOB SOURCES "${SOURCE_DIR}/*.c")

# Define the executable name
set(EXE_NAME main)
# Compile source files into an executable
add_executable(${EXE_NAME} ${SOURCES})

# Specify the C standard to be used
set_property(TARGET ${EXE_NAME} PROPERTY C_STANDARD 17)

# Link external libraries required by the project
target_link_libraries(${EXE_NAME} PRIVATE box2D raylib)

# Link windows-specific libraries (required by raylib)
if(WIN32)
    target_link_libraries(${EXE_NAME} PRIVATE gdi32 winmm)
endif()
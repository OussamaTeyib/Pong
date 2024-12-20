# Minimum CMake version required for this project
cmake_minimum_required(VERSION 3.5)

# Define the project name, version, and programming language
project(Pong VERSION 1.0.0 LANGUAGES C)

# Define the directory for source files
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
# Gather all .c files in the source directory
file(GLOB SOURCES "${SOURCE_DIR}/*.c")

# Define the executable name
set(EXE_NAME main)
# Compile source files into an executable
add_executable(${EXE_NAME} ${SOURCES})

# Link against essential libraries for Windows development
target_link_libraries(${EXE_NAME} PRIVATE raylib gdi32 winmm)
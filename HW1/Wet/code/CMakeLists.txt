cmake_minimum_required(VERSION 3.19)
project(smash)

set(CMAKE_CXX_STANDARD 14)

# Define variables for source files
set(SOURCES
    main.cpp
    smash/Smash.cpp
    signals/signals.cpp
)

# Use the variable in add_executable
add_executable(smash ${SOURCES})

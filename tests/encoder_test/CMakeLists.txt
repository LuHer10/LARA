cmake_minimum_required(VERSION 3.10)
project(robot VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the library subdirectory
add_subdirectory(../../robot ${CMAKE_BINARY_DIR}/robot_build)

# Define the executable
add_executable(robot
    main.cpp
)

# Link against the library
target_link_libraries(robot PRIVATE robotlib)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(encoders)

# Add the library subdirectory
add_subdirectory(../robot ${CMAKE_BINARY_DIR}/robot_build)

# Get all .cpp test files in this directory
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS *.cpp)

foreach(test_src ${TEST_SOURCES})
    # Get the file name without the directory or extension
    get_filename_component(test_name ${test_src} NAME_WE)

    # Create an executable per test file
    add_executable(${test_name} ${test_src})

    # Link with the robotlib
    target_link_libraries(${test_name} PRIVATE robotlib)
endforeach()


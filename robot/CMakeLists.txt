cmake_minimum_required(VERSION 3.10)
project(robot)

add_executable(${PROJECT_NAME} main.cpp)
add_library(roboclaw roboclaw.cpp)

target_link_libraries(${PROJECT_NAME} roboclaw)

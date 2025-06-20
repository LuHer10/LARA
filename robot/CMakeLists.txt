# ../mylib/CMakeLists.txt

add_library(robotlib
    roboclaw.cpp
)

target_include_directories(robotlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
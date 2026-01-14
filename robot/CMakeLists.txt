# ../mylib/CMakeLists.txt

add_library(robotlib
    roboclaw.cpp
    base.cpp
    network.cpp
    servo.cpp
)

target_include_directories(robotlib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(robotlib
    PUBLIC dynamixel_sdk
)
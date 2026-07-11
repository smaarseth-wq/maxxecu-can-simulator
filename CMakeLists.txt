cmake_minimum_required(VERSION 3.10)

project(MaxxECU_CAN_Simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    maxxecu_can_simulator

    src/main.cpp
    src/canbus.cpp
    src/simulator.cpp
)

target_compile_options(
    maxxecu_can_simulator
    PRIVATE
        -Wall
        -Wextra
        -O2
        -pthread
)

target_link_libraries(
    maxxecu_can_simulator
    pthread
)
